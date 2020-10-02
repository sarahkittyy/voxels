#include "o2d/sprite.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace o2d {

sprite::sprite(window& win)
	: m_win(win),
	  m_tex(nullptr) {
	transform().set_pos(glm::vec3(0.f, 0.f, 0.f));
}

sprite::~sprite() {
}

void sprite::load_texture(const std::string& path) {
	m_tex = &m_win.get_resource().tex(path);

	generate_mesh();
}

texture& sprite::get_texture() {
	return *m_tex;
}

void sprite::draw() {
	m_win.use_shader(SHADER::O2D);

	m_win.set_uniform(SHADER::O2D, "model", m_transform.to_mat4());
	m_win.set_uniform(SHADER::O2D, "proj",
					  glm::ortho(0.f, m_win.size().x, 0.f, m_win.size().y, -1.f, 1.f));

	if (m_tex == nullptr) throw std::runtime_error("No texture given to sprite.");
	m_tex->bind();

	m_mesh.draw();
}

glm::vec2 sprite::size() const {
	return m_tex->get_size();
}

util::transform& sprite::transform() {
	return m_transform;
}

void sprite::generate_mesh() {
	if (m_tex == nullptr) throw std::runtime_error("No texture given to sprite.");

	m_mesh.clear();

	unsigned int v1, v2, v3, v4;

	float wf = float(m_tex->get_size().x);
	float hf = float(m_tex->get_size().y);

	v1 = m_mesh.add_vertex(
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
	v2 = m_mesh.add_vertex(
		glm::vec3(wf, 0.0f, 0.0f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
	v3 = m_mesh.add_vertex(
		glm::vec3(wf, hf, 0.0f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
	v4 = m_mesh.add_vertex(
		glm::vec3(0.0f, hf, 0.0f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));

	m_mesh.add_triangle(v3, v1, v2);
	m_mesh.add_triangle(v3, v4, v1);

	m_mesh.generate();
}

}
