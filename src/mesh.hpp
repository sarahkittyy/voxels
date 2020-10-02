#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

// a single vertex
struct vertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
};

struct triangle {
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
};

// basic mesh
class mesh {
public:
	mesh();
	~mesh();

	unsigned int add_vertex(vertex v);
	unsigned int add_vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm);
	void add_triangle(unsigned int v1, unsigned int v2, unsigned int v3);
	void clear();

	void generate();

	// delete the internal mesh representation
	void del();

	// draw the mesh to the screen
	void draw();

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	std::vector<vertex> m_vertices;
	std::vector<triangle> m_tris;
};
