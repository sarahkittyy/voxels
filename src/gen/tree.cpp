#include "tree.hpp"

namespace gen {

tree::tree(superchunk* sc)
	: structure(sc, 5, 7, 5) {
}

tree::~tree() {
}

bool tree::can_place_at(int x, int y, int z) {
	return is_area_empty(
			   x, y, z,
			   0, height() - 1, 0) &&
		   area_exists(x, y, z,
					   width(), height(), depth());
}

void tree::place(int x, int y, int z) {
	// 2 height leaf prism from -w/2 - w/2
	for (int j = y + height() - 4; j < y + height() - 2; ++j) {
		// for every x and z
		for (int i = x; i < x + width(); ++i) {
			for (int k = z; k < z + depth(); ++k) {
				// set leaf block
				sc().get_voxel(i, j, k)->type = VOXEL_TYPE::LEAVES;
			}
		}
	}

	// now we just do w/3
	for (int j = y + height() - 2; j < y + height(); ++j) {
		// for every x and z
		for (int i = x + width() / 3; i < x + width() / 3 + 3; ++i) {
			for (int k = z + depth() / 3; k < z + depth() / 3 + 3; ++k) {
				// set leaf block
				sc().get_voxel(i, j, k)->type = VOXEL_TYPE::LEAVES;
			}
		}
	}


	// create the stem
	for (int j = y; j < y + height() - 2; ++j) {
		sc().get_voxel(x + width() / 2, j, z + width() / 2)->type = VOXEL_TYPE::LOG;
	}
}

}
