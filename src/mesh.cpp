#include "mesh.hpp"

mesh::mesh() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

mesh::~mesh() {
}

unsigned int mesh::add_vertex(vertex v) {
	m_vertices.push_back(v);
	return m_vertices.size() - 1;
}

unsigned int mesh::add_vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm) {
	return add_vertex(vertex({
		.pos  = pos,
		.uv	  = uv,
		.norm = norm,
	}));
}

void mesh::add_triangle(unsigned int v1, unsigned int v2, unsigned int v3) {
	m_tris.push_back({ .v1 = v1, .v2 = v2, .v3 = v3 });
}

void mesh::del() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void mesh::draw() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_tris.size() * 3, GL_UNSIGNED_INT, nullptr);
}

void mesh::clear() {
	m_vertices.clear();

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}

void mesh::generate() {
	glBindVertexArray(m_vao);
	vertex* const data = m_vertices.data();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex), data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
						  nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
						  (void*)sizeof(glm::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
						  (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_tris.size() * sizeof(triangle), m_tris.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
