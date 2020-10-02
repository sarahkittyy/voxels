#include "structure.hpp"

namespace gen {

structure::structure(superchunk* sc, int width, int height, int depth)
	: m_sc(sc),
	  m_width(width),
	  m_height(height),
	  m_depth(depth) {
}

structure::~structure() {
}

int structure::width() const {
	return m_width;
}

int structure::height() const {
	return m_height;
}

int structure::depth() const {
	return m_depth;
}

bool structure::can_place_at(int x, int y, int z) {
	return true;
}

void structure::place(int x, int y, int z) {
	m_sc->get_voxel(x, y, z)->type = VOXEL_TYPE::AIR;
}

bool structure::try_place(int x, int y, int z) {
	if (!can_place_at(x, y, z)) {
		return false;
	} else {
		place(x, y, z);
		return true;
	}
}

bool structure::area_exists(int x, int y, int z, int w, int h, int d) {
	for (int i = x; i <= x + w; ++i) {
		for (int j = y; j <= y + h; ++j) {
			for (int k = z; k <= z + d; ++k) {
				auto v = m_sc->get_voxel(i, j, k);
				if (v == nullptr) {
					return false;
				}
			}
		}
	}
	return true;
}

bool structure::is_area_empty(int x, int y, int z, int w, int h, int d) {
	for (int i = x; i <= x + w; ++i) {
		for (int j = y; j <= y + h; ++j) {
			for (int k = z; k <= z + d; ++k) {
				auto v = m_sc->get_voxel(i, j, k);
				if (v == nullptr || v->type != VOXEL_TYPE::AIR) {
					return false;
				}
			}
		}
	}
	return true;
}

bool structure::are_dimensions_empty(int x, int y, int z) {
	return is_area_empty(x, y, z, width(), height(), depth());
}

superchunk& structure::sc() {
	return *m_sc;
}

}
