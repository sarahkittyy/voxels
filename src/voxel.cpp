#include "voxel.hpp"

#include <unordered_map>

const float voxel::TEX_TILE_SIZE		 = 32.f;
const unsigned int voxel::TEX_GRID_WIDTH = 3;

voxel::voxel(VOXEL_TYPE type)
	: type(type) {
}

static const std::unordered_map<
	VOXEL_TYPE,
	const std::unordered_map<
		FACE,
		unsigned int>>
	UVMAP = {
		{ VOXEL_TYPE::DIRT,
		  {
			  { FACE::TOP, 0 },
			  { FACE::BOTTOM, 0 },
			  { FACE::LEFT, 0 },
			  { FACE::RIGHT, 0 },
			  { FACE::FRONT, 0 },
			  { FACE::BACK, 0 },
		  } },
		{ VOXEL_TYPE::GRASS,
		  {
			  { FACE::TOP, 2 },
			  { FACE::BOTTOM, 0 },
			  { FACE::LEFT, 1 },
			  { FACE::RIGHT, 1 },
			  { FACE::FRONT, 1 },
			  { FACE::BACK, 1 },
		  } },
		{ VOXEL_TYPE::LOG,
		  {
			  { FACE::TOP, 4 },
			  { FACE::BOTTOM, 4 },
			  { FACE::LEFT, 3 },
			  { FACE::RIGHT, 3 },
			  { FACE::FRONT, 3 },
			  { FACE::BACK, 3 },
		  } },
		{ VOXEL_TYPE::LEAVES,
		  {
			  { FACE::TOP, 5 },
			  { FACE::BOTTOM, 5 },
			  { FACE::LEFT, 5 },
			  { FACE::RIGHT, 5 },
			  { FACE::FRONT, 5 },
			  { FACE::BACK, 5 },
		  } }
	};

std::array<glm::vec2, 4> voxel::get_uv(FACE f) {
	// get id of voxel given type
	unsigned int id;

	if (UVMAP.find(type) == UVMAP.end()) {
		return {
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 0.f)
		};
	}
	id = UVMAP.at(type).at(f);

	// get the positions in the texture
	unsigned int txi, tyi;
	txi = id % TEX_GRID_WIDTH;
	tyi = id / TEX_GRID_WIDTH;

	// convert to absolute texture positions
	float tx, ty;
	tx = txi * TEX_TILE_SIZE;
	ty = tyi * TEX_TILE_SIZE;

	// normalize
	float txn, tyn;
	txn = (tx + 0.5f) / (TEX_TILE_SIZE * TEX_GRID_WIDTH);
	tyn = (ty + 0.5f) / (TEX_TILE_SIZE * TEX_GRID_WIDTH);

	// increments to create the uv square
	float xv, yv;
	xv = 1 / float(TEX_GRID_WIDTH);
	yv = 1 / float(TEX_GRID_WIDTH);

	xv -= 0.5f / (TEX_TILE_SIZE * TEX_GRID_WIDTH);
	yv -= 0.5f / (TEX_TILE_SIZE * TEX_GRID_WIDTH);

	// convert to uvs
	std::array<glm::vec2, 4> uvs;

	uvs[0] = glm::vec2(txn, tyn);
	uvs[1] = glm::vec2(txn + xv, tyn);
	uvs[2] = glm::vec2(txn + xv, tyn + yv);
	uvs[3] = glm::vec2(txn, tyn + yv);

	return uvs;
}
