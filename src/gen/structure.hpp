#pragma once

#include <superchunk.hpp>
#include <voxel.hpp>

namespace gen {

// an abstract structure class that's placeable in the world during generation
// NOTE: assume that
class structure {
public:
	structure(superchunk* sc, int width, int height, int depth);
	virtual ~structure();

	// dimensions of the structure
	int width() const;
	int height() const;
	int depth() const;

	// can the structure be placed at the given xyz coords
	virtual bool can_place_at(int x, int y, int z);

	// place the structure at the given coords
	virtual void place(int x, int y, int z);

	// try to place a structure at the given coords. returns false if !can_place_at
	bool try_place(int x, int y, int z);

protected:
	/* hewe shall lie all helper functions to access and test voxel locations */

	// test if the area isn't OOB
	bool area_exists(int x, int y, int z, int w, int h, int d);

	// test if the given area is empty.
	bool is_area_empty(int x, int y, int z, int w, int h, int d);

	// checks if the whole box from x, y, z of dimensions w, h, d is nothing but AIR
	bool are_dimensions_empty(int x, int y, int z);

	// for the child to access the parent superchunk
	superchunk& sc();

private:
	// superchunk to write to
	superchunk* m_sc;

	// size of the structure
	int m_width, m_height, m_depth;
};

}
