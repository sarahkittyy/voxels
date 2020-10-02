#pragma once

#include <glm/glm.hpp>

#include "util/clock.hpp"
#include "util/events.hpp"
#include "window.hpp"

// abstract camera class
class camera : public util::subscriber {
public:
	camera(window* win);
	virtual ~camera();

	// update the proj & view matrices :D
	void update();

	void set_fov(float new_fov);
	float get_fov() const;

	virtual glm::mat4 get_view() const = 0;
	glm::mat4 get_proj() const;
	virtual glm::vec3 get_pos() const = 0;

	// get the invert of the proj * view matrix
	glm::mat4 invert() const;

protected:
	// up vector for all cameras
	static const glm::vec3 UP;

	// updated before setting the matrices
	virtual void child_update();

	// get a reference to the internal window
	window& get_window();

private:
	// internal pointer to the window
	window* m_win;

	// camera's fov
	float m_fov;
};

// simple camera, simply has a position & a direction -- can rotate.
class static_camera : public camera {
public:
	static_camera(window* win);

	void set_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;
	void move(glm::vec3 dp);

	void set_dir(glm::vec3 dir);
	glm::vec3 get_dir() const;
	void translate(glm::vec3 dp);
	void rotate(float angle, glm::vec3 axis);
	void look_at(glm::vec3 pos);

	glm::mat4 get_view() const;

private:
	// position and direction of camera
	glm::vec3 m_pos;
	glm::vec3 m_dir;
};

// camera that listens to input and flies around
class free_camera : public camera {
public:
	free_camera(window* win);

	void set_pos(glm::vec3 pos);

	glm::vec3 get_pos() const;
	glm::mat4 get_view() const;

protected:
	void child_update();

private:
	// camera position & heading
	glm::vec3 m_pos;
	glm::vec3 m_dir;
	float m_yaw;
	float m_pitch;
	glm::vec3 m_up;

	void translate(glm::vec3 disp);

	// speed to move the camera
	const float m_speed = 6.f;
	// speed to rotate the camera
	const float m_rotate_speed = 0.1f;

	// for delta time computing
	util::clock m_clock;

	// keycodes for WASD
	int m_left_key;
	int m_right_key;
	int m_forward_key;
	int m_backward_key;
	int m_up_key;
	int m_down_key;

	// previous mouse position
	float m_mouse_px;
	float m_mouse_py;

	// most recent mouse position
	float m_mouse_x;
	float m_mouse_y;
};
