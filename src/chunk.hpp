#pragma once

#include <noise/noise.h>
#include <array>

#include "mesh.hpp"
#include "texture.hpp"
#include "util/transform.hpp"
#include "voxel.hpp"
#include "window.hpp"

class superchunk;

// one 16x16x16 chunk of voxels
class chunk {
public:
	chunk(superchunk& parent, window& win);
	~chunk();

	void draw();

	// delete all internal opengl data
	void del();

	// set the chunk's position in the world
	void set_world_index(int x, int y, int z);

	// compute mesh data
	void setup();

	// get a voxel relative to this chunk
	voxel* get_voxel_local(int x, int y, int z);

	static const int WIDTH;
	static const int HEIGHT;
	static const int DEPTH;

	static const int CUBE_SIZE;

private:
	// main app window
	window& m_win;

	// parent superchunk
	superchunk& m_parent;
	superchunk& parent();

	voxel*** m_voxels;

	// compute the mesh
	void compute_mesh();

	mesh m_mesh;

	void create_cube(int x, int y, int z, VOXEL_TYPE type);
	void gen_face(
		glm::vec3 norm,
		std::array<glm::vec3, 4> vs,
		std::array<glm::vec2, 4> uvs,
		bool reverse_wind);

	// internal mesh transform
	util::transform m_transform;

	// absolute world pos
	glm::vec3 m_apos;
	voxel* get_voxel(int x, int y, int z);

	//for binding on mesh render
	texture& m_tex;
};
