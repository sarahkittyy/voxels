#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/glm.hpp>
#include <unordered_map>

#include "color.hpp"
#include "resource.hpp"
#include "shader.hpp"
#include "util/events.hpp"
#include "util/transform.hpp"

enum class EVENT {
	/// int width, height
	RESIZE,
	/// int key, scancode, action, mods
	INPUT,
	/// double x, y
	MOUSE_MOVE,
	/// int button, action, mods
	MOUSE_INPUT,
};

// all the different types of shader programs available
enum class SHADER {
	NORMAL,
	O2D,
};

class window : public util::evtmgr {
public:
	typedef std::function<void(util::data)> cbfn_t;

	window(unsigned int width, unsigned int height, const char* title);
	~window();

	// should the window close?
	bool should_close() const;

	// clear the gl buffer
	void clear(color cc);

	// swap buffers
	void swap();

	// set a uniform in the shader to a value
	template <typename T>
	void set_uniform(SHADER s, const char* uniform, const T&& data);

	// shader retrieval
	shader* shdr(SHADER s);

	// set the shader to use
	void use_shader(SHADER s);

	// get the window's position
	glm::vec2 pos() const;

	// get the window's size
	glm::vec2 size() const;

	// get the internal glfw window handle
	GLFWwindow* handle();

	// return the window's resource manager
	static resource& get_resource();

private:
	// main window object
	GLFWwindow* m_window;

	// resourcemanager
	static resource m_resource;

	// for GLFW callbacks
	static std::unordered_map<GLFWwindow*, window*> m_wmap;

	friend void _on_resize_cb(GLFWwindow*, int, int);
	friend void _on_input_cb(GLFWwindow*, int, int, int, int);
	friend void _on_mouse_move_cb(GLFWwindow*, double, double);
	friend void _on_mouse_input_cb(GLFWwindow*, int, int, int);

	void on_resize(int nw, int nh);
	void on_input(int key, int scancode, int action, int mods);
	void on_mouse_move(double x, double y);
	void on_mouse_input(int button, int action, int mods);

	// main window shader
	shader* m_normal_shader;
	// 2d shader
	shader* m_2d_shader;
	// current shader
	SHADER m_curr_shader;

	// has the window been initialized already?
	static bool initd;
};
