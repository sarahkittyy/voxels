#include "chunk.hpp"
#include "superchunk.hpp"

#include <cmath>
#include <iostream>

const int chunk::WIDTH	= 16;
const int chunk::HEIGHT = 16;
const int chunk::DEPTH	= 16;

const int chunk::CUBE_SIZE = 1;

chunk::chunk(superchunk& parent, window& win)
	: m_win(win),
	  m_parent(parent),
	  m_apos(0, 0, 0),
	  m_tex(m_win.get_resource().tex("assets/atlas.png")) {
	m_tex.set_filter_mode(FILTER_MODE::NEAREST);
	m_tex.set_wrap_mode(WRAP_MODE::REPEAT);

	m_voxels = new voxel**[DEPTH];
	for (int x = 0; x < WIDTH; ++x) {
		m_voxels[x] = new voxel*[HEIGHT];
		for (int y = 0; y < HEIGHT; ++y) {
			m_voxels[x][y] = new voxel[WIDTH];
			for (int z = 0; z < DEPTH; ++z) {
				m_voxels[x][y][z] = voxel(VOXEL_TYPE::AIR);
			}
		}
	}
}

chunk::~chunk() {
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			delete[] m_voxels[x][y];
		}
		delete[] m_voxels[x];
	}
	delete[] m_voxels;
}

superchunk& chunk::parent() {
	return m_parent;
}

void chunk::setup() {
	compute_mesh();
}

void chunk::set_world_index(int x, int y, int z) {
	m_apos = glm::vec3(x, y, z);
	m_transform.set_pos(glm::vec3(
		m_apos.x * WIDTH,
		m_apos.y * HEIGHT,
		m_apos.z * DEPTH));
}

voxel* chunk::get_voxel(int x, int y, int z) {
	if (x >= 0 && x < WIDTH &&
		y >= 0 && y < HEIGHT &&
		z >= 0 && z < DEPTH) {
		return &m_voxels[x][y][z];
	} else {
		int nx, ny, nz;
		nx = x + m_apos.x * WIDTH;
		ny = y + m_apos.y * HEIGHT;
		nz = z + m_apos.z * DEPTH;
		return parent().get_voxel(nx, ny, nz);
	}
}

voxel* chunk::get_voxel_local(int x, int y, int z) {
	if (x >= 0 && x < WIDTH &&
		y >= 0 && y < HEIGHT &&
		z >= 0 && z < DEPTH) {
		return &m_voxels[x][y][z];
	} else {
		return nullptr;
	}
}

void chunk::compute_mesh() {
	m_mesh.clear();
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			for (int z = 0; z < DEPTH; ++z) {
				if (get_voxel(x, y, z)->type == VOXEL_TYPE::AIR) {
					continue;
				}

				create_cube(x, y, z, get_voxel(x, y, z)->type);
			}
		}
	}
}

void chunk::create_cube(int xi, int yi, int zi, VOXEL_TYPE type) {
	voxel* v = &m_voxels[xi][yi][zi];
	*v		 = voxel(type);

	float x = (float)xi;
	float y = (float)yi;
	float z = (float)zi;

	// TOP FACE
	glm::vec3 v5 = glm::vec3(x, y - CUBE_SIZE, z);
	glm::vec3 v6 = glm::vec3(x + CUBE_SIZE, y - CUBE_SIZE, z);
	glm::vec3 v7 = glm::vec3(x + CUBE_SIZE, y - CUBE_SIZE, z + CUBE_SIZE);
	glm::vec3 v8 = glm::vec3(x, y - CUBE_SIZE, z + CUBE_SIZE);
	// BOT FACE
	glm::vec3 v1 = glm::vec3(x, y, z);
	glm::vec3 v2 = glm::vec3(x + CUBE_SIZE, y, z);
	glm::vec3 v3 = glm::vec3(x + CUBE_SIZE, y, z + CUBE_SIZE);
	glm::vec3 v4 = glm::vec3(x, y, z + CUBE_SIZE);

	glm::vec3 n;
	std::array<glm::vec2, 4> uvs = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
	};

	// top
	if (auto vb = get_voxel(xi, yi + 1, zi);
		!vb || vb->type == VOXEL_TYPE::AIR) {

		n	= glm::vec3(0.f, 1.f, 0.f);
		uvs = v->get_uv(FACE::TOP);
		gen_face(n, { v1, v2, v3, v4 }, uvs, true);
	}
	// bot
	if (auto vb = get_voxel(xi, yi - 1, zi);
		!vb || vb->type == VOXEL_TYPE::AIR) {

		n	= glm::vec3(0.f, -1.f, 0.f);
		uvs = v->get_uv(FACE::BOTTOM);
		gen_face(n, { v5, v6, v7, v8 }, uvs, false);
	}

	// front
	if (auto vb = get_voxel(xi, yi, zi - 1);
		!vb || vb->type == VOXEL_TYPE::AIR) {
		n	= glm::vec3(0.f, 0.f, 1.f);
		uvs = v->get_uv(FACE::FRONT);
		gen_face(n, { v1, v2, v6, v5 }, uvs, false);
	}
	// back
	if (auto vb = get_voxel(xi, yi, zi + 1);
		!vb || vb->type == VOXEL_TYPE::AIR) {
		n	= glm::vec3(0.f, 0.f, -1.f);
		uvs = v->get_uv(FACE::BACK);
		gen_face(n, { v3, v4, v8, v7 }, uvs, false);
	}

	// left
	if (auto vb = get_voxel(xi - 1, yi, zi);
		!vb || vb->type == VOXEL_TYPE::AIR) {
		n	= glm::vec3(-1.f, 0.f, 0.f);
		uvs = v->get_uv(FACE::LEFT);
		gen_face(n, { v1, v4, v8, v5 }, uvs, true);
	}
	// right
	if (auto vb = get_voxel(xi + 1, yi, zi);
		!vb || vb->type == VOXEL_TYPE::AIR) {
		n	= glm::vec3(1.f, 0.f, 0.f);
		uvs = v->get_uv(FACE::RIGHT);
		gen_face(n, { v2, v3, v7, v6 }, uvs, false);
	}

	m_mesh.generate();
}

void chunk::gen_face(
	glm::vec3 n,
	std::array<glm::vec3, 4> vs,
	std::array<glm::vec2, 4> uvs,
	bool reverse_wind) {

	int i1, i2, i3, i4;

	i1 = m_mesh.add_vertex(vs[0], uvs[0], n);
	i2 = m_mesh.add_vertex(vs[1], uvs[1], n);
	i3 = m_mesh.add_vertex(vs[2], uvs[2], n);
	i4 = m_mesh.add_vertex(vs[3], uvs[3], n);

	if (!reverse_wind) {
		m_mesh.add_triangle(i1, i2, i3);
		m_mesh.add_triangle(i1, i3, i4);
	} else {
		m_mesh.add_triangle(i1, i4, i3);
		m_mesh.add_triangle(i1, i3, i2);
	}
}

void chunk::draw() {
	m_win.use_shader(SHADER::NORMAL);

	m_tex.bind();
	m_win.set_uniform(SHADER::NORMAL, "model", m_transform.to_mat4());

	m_mesh.draw();
}

void chunk::del() {
	m_mesh.del();
}
