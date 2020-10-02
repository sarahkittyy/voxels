#include "app.hpp"

#include "o2d/sprite.hpp"
#include "superchunk.hpp"

app::app()
	: m_win(800, 600, "voxels"),
	  m_cam(&m_win) {
	init(&m_win);

	m_cam.set_fov(45.f);
	m_cam.set_pos(glm::vec3(18.f, 48.f, -1.f));

	m_win.set_uniform(SHADER::NORMAL, "lightPos", glm::vec3(64.f, 40.f, -64.f));
	m_win.set_uniform(SHADER::NORMAL, "lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	hook((int)EVENT::INPUT, [this](util::data d) {
		int key = d.get<int>("key");
		//int scancode = d.get<int>("scancode");
		int action = d.get<int>("action");
		//int mods	 = d.get<int>("mods");

		if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
			int mode = glfwGetInputMode(m_win.handle(), GLFW_CURSOR);
			if (mode == GLFW_CURSOR_DISABLED) {
				glfwSetInputMode(m_win.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			} else {
				glfwSetInputMode(m_win.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	});
}

int app::run() {

	superchunk sc(m_win);
	o2d::sprite cursor(m_win);
	cursor.load_texture("assets/cursor.png");
	cursor.get_texture().set_filter_mode(FILTER_MODE::NEAREST);

	auto cursor_update = [&cursor, this]() {
		cursor.transform().set_pos(
			glm::vec3(
				(m_win.size().x + cursor.size().x) / 2.f,
				(m_win.size().y + cursor.size().y) / 2.f,
				0.0f));

		cursor.transform().set_scale(glm::vec3(1.5f, 1.5f, 0.f));
	};

	cursor_update();

	hook((int)EVENT::RESIZE, [&cursor_update](util::data d) {
		cursor_update();
	});

	while (!m_win.should_close()) {
		m_cam.update();

		m_win.clear(color(0.2f, 0.2f, 0.75f));

		sc.draw();
		cursor.draw();

		m_win.swap();
	}

	glfwTerminate();

	return 0;
}

int main() {
	app a;
	return a.run();
}
