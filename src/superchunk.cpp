#include "superchunk.hpp"

#include <glm/glm.hpp>
#include "gen/tree.hpp"

#include <iostream>

const int superchunk::SUPERCHUNK_WIDTH	= 3;
const int superchunk::SUPERCHUNK_HEIGHT = 3;
const int superchunk::SUPERCHUNK_DEPTH	= 3;

const int superchunk::CHUNKS_PER_FRAME = 1;
const int superchunk::RENDER_DISTANCE  = 4;

superchunk::superchunk(window& win)
	: m_win(win) {
	m_cl_final.resize(SUPERCHUNK_WIDTH * SUPERCHUNK_HEIGHT * SUPERCHUNK_DEPTH);
	for (int i = 0; i < m_cl_final.size(); ++i) {
		m_cl_final[i] = std::shared_ptr<chunk>(new chunk(*this, m_win));
	}

	// compute noise stuff
	configure_noise();

	// generate the terrain
	generate_terrain();

	// TODO: split this into threads
	for (int x = 0; x < SUPERCHUNK_WIDTH; ++x) {
		for (int y = SUPERCHUNK_HEIGHT - 1; y >= 0; --y) {
			for (int z = 0; z < SUPERCHUNK_DEPTH; ++z) {
				std::shared_ptr<chunk>& cc = m_cl_final[pos_to_index(x, y, z)];
				cc->set_world_index(x, y, z);
				cc->setup();
			}
		}
	}
}

superchunk::~superchunk() {
}

void superchunk::update() {
}

void superchunk::draw() {
	m_win.use_shader(SHADER::NORMAL);

	for (size_t i = 0; i < m_cl_final.size(); ++i) {
		if (m_cl_final[i]) {
			m_cl_final[i]->draw();
		}
	}
}

void superchunk::configure_noise() {
	m_pnoise.SetSeed(time(NULL));

	utils::NoiseMapBuilderPlane hmb;
	hmb.SetSourceModule(m_pnoise);
	hmb.SetDestNoiseMap(m_heightmap);
	hmb.SetDestSize(SUPERCHUNK_WIDTH * chunk::WIDTH, SUPERCHUNK_DEPTH * chunk::DEPTH);
	hmb.SetBounds(2, 6, 1, 5);
	hmb.Build();
}

void superchunk::generate_terrain() {
	// for every x/z pos
	for (int x = 0; x < SUPERCHUNK_WIDTH * chunk::WIDTH; ++x) {
		for (int z = 0; z < SUPERCHUNK_DEPTH * chunk::DEPTH; ++z) {
			// compute the height of the terrain at this point
			float height =
				m_heightmap.GetValue(x, z);
			// define a new range
			float lo = 34;
			float hi = 40;
			height += 1;	 // 0 - 2
			height /= 2.f;	 // 0 - 1
			height *= (hi - lo);
			height += lo;		   // lo - hi
			int h = int(height);   // floor it.

			// x, h, z should be grass, below it should be dirt, above should be air.

			// so for every y......
			for (int y = 0; y < SUPERCHUNK_HEIGHT * chunk::HEIGHT; ++y) {
				if (y > h) {
					// air, but i don't wanna overwrite existing tiles
				} else if (y == h) {
					get_voxel(x, y, z)->type = VOXEL_TYPE::GRASS;
				} else {
					get_voxel(x, y, z)->type = VOXEL_TYPE::DIRT;
				}
			}

			// optionally add a tree here
			if (rand() % 150 < 5) {
				gen::tree(this).try_place(x - 2, h + 1, z - 2);
			}
		}
	}

	// add some struct
}

voxel* superchunk::get_voxel(int x, int y, int z) {
	// get the chunk that the voxel belongs to
	int cx, cy, cz;
	cx = x / chunk::WIDTH;
	cy = y / chunk::HEIGHT;
	cz = z / chunk::DEPTH;

	if (cx < 0 || cx >= SUPERCHUNK_WIDTH ||
		cy < 0 || cy >= SUPERCHUNK_HEIGHT ||
		cz < 0 || cz >= SUPERCHUNK_DEPTH) {
		return nullptr;
	}

	chunk& c = *m_cl_final[pos_to_index(cx, cy, cz)];

	// get the voxel in that chunk
	int vx, vy, vz;
	vx = x % chunk::WIDTH;
	vy = y % chunk::HEIGHT;
	vz = z % chunk::DEPTH;

	return c.get_voxel_local(vx, vy, vz);
}

constexpr int superchunk::pos_to_index(int x, int y, int z) {
	return x + (y * SUPERCHUNK_WIDTH) + (z * SUPERCHUNK_WIDTH * SUPERCHUNK_HEIGHT);
}
