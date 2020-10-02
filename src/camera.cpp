#include "camera.hpp"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

const glm::vec3 camera::UP = glm::vec3(0.f, 1.f, 0.f);

camera::camera(window* win)
	: m_win(win),
	  m_fov(45.f) {
	init(m_win);
}

camera::~camera() {
}

void camera::update() {
	child_update();
	m_win->set_uniform(SHADER::NORMAL, "proj", get_proj());
	m_win->set_uniform(SHADER::NORMAL, "view", get_view());
	m_win->set_uniform(SHADER::NORMAL, "camPos", get_pos());
}

void camera::child_update() {
}

void camera::set_fov(float new_fov) {
	m_fov = new_fov;
}

float camera::get_fov() const {
	return m_fov;
}

glm::mat4 camera::get_proj() const {
	return glm::perspective(m_fov, m_win->size().x / m_win->size().y, 0.1f, 500.f);
}

glm::mat4 camera::invert() const {
	return glm::inverse(get_proj() * get_view());
}

window& camera::get_window() {
	return *m_win;
}

////////////////////////////////////////////////////

static_camera::static_camera(window* win)
	: camera(win),
	  m_pos(0.f, 0.f, 0.f),
	  m_dir(0.f, 0.f, 0.f) {
}

void static_camera::set_pos(glm::vec3 pos) {
	m_pos = pos;
}

glm::vec3 static_camera::get_pos() const {
	return m_pos;
}

void static_camera::move(glm::vec3 dp) {
	m_pos += dp;
}

void static_camera::set_dir(glm::vec3 dir) {
	m_dir = dir;
}

glm::vec3 static_camera::get_dir() const {
	return m_dir;
}

void static_camera::translate(glm::vec3 disp) {
	m_dir += disp;
}

void static_camera::rotate(float angle, glm::vec3 axis) {
	m_dir = glm::rotate(m_dir, angle * 0.1f, axis);
}

void static_camera::look_at(glm::vec3 pos) {
	m_dir = pos - m_pos;
}

glm::mat4 static_camera::get_view() const {
	return glm::lookAt(m_pos, m_pos + m_dir, UP);
}

////////////////////////////////////////////////////

free_camera::free_camera(window* win)
	: camera(win),
	  m_pos(0.f, 0.f, 0.f),
	  m_dir(0.f, 0.f, 1.f),
	  m_yaw(0.f),
	  m_pitch(0.f),
	  m_up(0.f, 1.f, 0.f),
	  m_left_key(GLFW_KEY_A),
	  m_right_key(GLFW_KEY_D),
	  m_forward_key(GLFW_KEY_W),
	  m_backward_key(GLFW_KEY_S),
	  m_up_key(GLFW_KEY_SPACE),
	  m_down_key(GLFW_KEY_LEFT_SHIFT) {

	hook((int)EVENT::MOUSE_MOVE, [this](util::data d) {
		float x	  = (float)d.get<double>("x");
		float y	  = (float)d.get<double>("y");
		m_mouse_x = x;
		m_mouse_y = y;
	});

	double x;
	double y;
	glfwGetCursorPos(win->handle(), &x, &y);
	m_mouse_x = m_mouse_px = int(x);
	m_mouse_y = m_mouse_py = int(y);
}

void free_camera::set_pos(glm::vec3 pos) {
	m_pos = pos;
}

void free_camera::child_update() {
	float dt   = m_clock.restart().as_seconds();
	float h_pi = 3.14f / 2.f;

	// calculate mouse dp
	float dx = m_mouse_x - m_mouse_px;
	float dy = m_mouse_y - m_mouse_py;

	dx *= dt * -m_rotate_speed;
	dy *= dt * -m_rotate_speed;

	m_yaw += dx;
	m_pitch += dy;

	m_pitch = glm::clamp(m_pitch, -h_pi, h_pi);

	// update most recent mouse pos
	m_mouse_px = m_mouse_x;
	m_mouse_py = m_mouse_y;

	m_dir = glm::vec3(
		std::cos(m_pitch) * std::sin(m_yaw),
		std::sin(m_pitch),
		std::cos(m_pitch) * std::cos(m_yaw));
	glm::vec3 n_dir = glm::normalize(m_dir);

	glm::vec3 right(
		std::sin(m_yaw - h_pi),
		0,
		std::cos(m_yaw - h_pi));

	glm::vec3 fwd = glm::rotate(right, h_pi, glm::vec3(0.f, 1.f, 0.f));

	m_up = glm::cross(right, n_dir);

	// handle motion
	if (glfwGetKey(get_window().handle(), m_left_key) == GLFW_PRESS) {
		translate(right * -m_speed * dt);
	}
	if (glfwGetKey(get_window().handle(), m_right_key) == GLFW_PRESS) {
		translate(right * m_speed * dt);
	}
	if (glfwGetKey(get_window().handle(), m_forward_key) == GLFW_PRESS) {
		translate(fwd * dt * m_speed);
	}
	if (glfwGetKey(get_window().handle(), m_backward_key) == GLFW_PRESS) {
		translate(fwd * dt * -m_speed);
	}
	if (glfwGetKey(get_window().handle(), m_up_key) == GLFW_PRESS) {
		translate(UP * dt * m_speed);
	}
	if (glfwGetKey(get_window().handle(), m_down_key) == GLFW_PRESS) {
		translate(UP * dt * -m_speed);
	}
}

glm::vec3 free_camera::get_pos() const {
	return m_pos;
}

glm::mat4 free_camera::get_view() const {
	return glm::lookAt(m_pos, m_pos + m_dir, m_up);
}

void free_camera::translate(glm::vec3 disp) {
	m_pos += disp;
}
