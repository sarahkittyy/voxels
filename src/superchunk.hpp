#pragma once

#include <noise/noise.h>
#include <noiseutils.h>
#include <memory>
#include <vector>

#include "chunk.hpp"
#include "window.hpp"

// meta-chunk to store a 32x32x32 group of chunks
class superchunk {
public:
	superchunk(window& win);
	~superchunk();

	// updates rendered chunks
	void update();
	// draws all chunks to the screen
	void draw();

	// get voxel at a given global xyz
	voxel* get_voxel(int x, int y, int z);

private:
	// app window
	window& m_win;

	// chunk generation noise
	utils::NoiseMap m_heightmap;
	// main perlin noise module
	noise::module::Perlin m_pnoise;

	// chunklists
	std::vector<std::shared_ptr<chunk>> m_cl_final;

	// chunklist updates
	void load_new_chunks();
	void unload_old_chunks();
	void update_render_list();

	void configure_noise();
	void generate_terrain();

	constexpr int pos_to_index(int x, int y, int z);

	// superchunk size
	static const int SUPERCHUNK_WIDTH;
	static const int SUPERCHUNK_HEIGHT;
	static const int SUPERCHUNK_DEPTH;

	// other static vars
	static const int CHUNKS_PER_FRAME;
	static const int RENDER_DISTANCE;
};
