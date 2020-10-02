#include "window.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

#include <algorithm>

#include "shaders/o2d.hpp"
#include "shaders/regular.hpp"

resource window::m_resource;
std::unordered_map<GLFWwindow*, window*> window::m_wmap = {};
bool window::initd										= false;

/////////////////////callback handlers/////////////////////////

void _on_resize_cb(GLFWwindow* w, int nw, int nh) {
	window::m_wmap[w]->on_resize(nw, nh);
}

void _on_input_cb(GLFWwindow* w, int key, int scancode, int action, int mods) {
	window::m_wmap[w]->on_input(key, scancode, action, mods);
}

void _on_mouse_move_cb(GLFWwindow* w, double x, double y) {
	window::m_wmap[w]->on_mouse_move(x, y);
}

void _on_mouse_input_cb(GLFWwindow* w, int button, int action, int mods) {
	window::m_wmap[w]->on_mouse_input(button, action, mods);
}

void window::on_resize(int nw, int nh) {
	glViewport(0, 0, nw, nh);
	emit((int)EVENT::RESIZE, util::data().set("width", nw).set("height", nh));
}

void window::on_input(int key, int scancode, int action, int mods) {
	emit(
		(int)EVENT::INPUT,
		util::data()
			.set("key", key)
			.set("scancode", scancode)
			.set("action", action)
			.set("mods", mods));
}

void window::on_mouse_move(double x, double y) {
	emit((int)EVENT::MOUSE_MOVE, util::data().set("x", x).set("y", y));
}

void window::on_mouse_input(int button, int action, int mods) {
	emit(
		(int)EVENT::MOUSE_INPUT,
		util::data()
			.set("button", button)
			.set("action", action)
			.set("mods", mods));
}

////////////////template instantiation//////////////////////////////

template <>
void window::set_uniform(SHADER s, const char* uniform, const glm::mat4&& mat4) {
	GLuint loc = glGetUniformLocation(shdr(s)->prog(), uniform);
	shdr(s)->use();
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat4[0][0]);
	shdr(m_curr_shader)->use();
}

template <>
void window::set_uniform(SHADER s, const char* uniform, const glm::mat2&& mat2) {
	GLuint loc = glGetUniformLocation(shdr(s)->prog(), uniform);
	shdr(s)->use();
	glUniformMatrix2fv(loc, 1, GL_FALSE, &mat2[0][0]);
	shdr(m_curr_shader)->use();
}


template <>
void window::set_uniform(SHADER s, const char* uniform, const glm::vec3&& vec3) {
	GLuint loc = glGetUniformLocation(shdr(s)->prog(), uniform);
	shdr(s)->use();
	glUniform3fv(loc, 1, &vec3[0]);
	shdr(m_curr_shader)->use();
}

template <>
void window::set_uniform(SHADER s, const char* uniform, const float&& v) {
	GLuint loc = glGetUniformLocation(shdr(s)->prog(), uniform);
	shdr(s)->use();
	glUniform1fv(loc, 1, &v);
	shdr(m_curr_shader)->use();
}

////////////////actual window class///////////////////////////

window::window(unsigned int width, unsigned int height, const char* title) {
	if (!initd) {
		glewExperimental = true;
		if (!glfwInit()) {
			throw std::runtime_error("Could not initialize glfw.");
		}
	}

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(m_window);

	if (!initd) {
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			throw std::runtime_error("Could not initialize glew.");
		}
	}

	initd = true;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CCW);

	// default shader
	m_normal_shader = new shader(shaders::regular_vs(), shaders::regular_fs());
	m_normal_shader->use();

	// 2d shader
	m_2d_shader = new shader(shaders::o2d_vs(), shaders::o2d_fs());

	use_shader(SHADER::NORMAL);

	// default mvp matrix stats
	set_uniform(SHADER::NORMAL, "model", glm::mat4(1.0f));
	set_uniform(SHADER::NORMAL, "proj",
				glm::perspective(45.f, float(width) / float(height), 0.1f, 500.f));
	set_uniform(SHADER::NORMAL, "view",
				glm::lookAt(
					glm::vec3(-8.f, -8.f, -8.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0, 1, 0)));

	glViewport(0, 0, width, height);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(m_window, _on_input_cb);
	glfwSetWindowSizeCallback(m_window, _on_resize_cb);
	glfwSetCursorPosCallback(m_window, _on_mouse_move_cb);
	glfwSetMouseButtonCallback(m_window, _on_mouse_input_cb);

	m_wmap[m_window] = this;
}

window::~window() {
	m_wmap.erase(m_window);
	delete m_normal_shader;
	delete m_2d_shader;
	glfwDestroyWindow(m_window);
}

void window::use_shader(SHADER s) {
	m_curr_shader = s;
	shdr(s)->use();
}

glm::vec2 window::pos() const {
	int x, y;
	glfwGetWindowPos(m_window, &x, &y);
	return glm::vec2(x, y);
}

glm::vec2 window::size() const {
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	return glm::vec2(float(w), float(h));
}

void window::clear(color cc) {
	glfwPollEvents();

	glClearColor(cc.r, cc.g, cc.b, cc.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::swap() {
	glfwSwapBuffers(m_window);
}

GLFWwindow* window::handle() {
	return m_window;
}

resource& window::get_resource() {
	return m_resource;
}

shader* window::shdr(SHADER s) {
	switch (s) {
	case SHADER::NORMAL:
		return m_normal_shader;
	case SHADER::O2D:
		return m_2d_shader;
	default:
		throw std::runtime_error("Shader not found!");
	}
}

bool window::should_close() const {
	return glfwWindowShouldClose(m_window);
}
