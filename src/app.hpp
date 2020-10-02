#pragma once

#include "camera.hpp"
#include "util/events.hpp"
#include "window.hpp"

// main application class
class app : public util::subscriber {
public:
	app();

	int run();	 // run the app, returns the exit code

private:
	window m_win;
	free_camera m_cam;
};
