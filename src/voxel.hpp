#pragma once

#include <array>
#include <glm/glm.hpp>

// all voxel block types
enum class VOXEL_TYPE {
	AIR,
	DIRT,
	GRASS,
	LOG,
	LEAVES,
};

// all 6 faces of a cube
enum class FACE {
	TOP,	  // +y
	BOTTOM,	  // -y
	LEFT,	  // -x
	RIGHT,	  // +x
	FRONT,	  // -z
	BACK,	  // +z
};

// all 6 directions in 3d space
enum class DIR {
	UP,		   // +y
	DOWN,	   // -y
	LEFT,	   // -x
	RIGHT,	   // +x
	FORWARD,   // -z
	BACK,	   // +z
};

// voxel data type, stores no rendering info, just the voxel's type
class voxel {
public:
	voxel(VOXEL_TYPE type = VOXEL_TYPE::AIR);

	// the voxel's type
	VOXEL_TYPE type;

	// get all four uv coords for the given face
	std::array<glm::vec2, 4> get_uv(FACE f);

	// size of a single texture in the atlas
	static const float TEX_TILE_SIZE;
	// width of the grid of textures in the atlas
	static const unsigned int TEX_GRID_WIDTH;
};
