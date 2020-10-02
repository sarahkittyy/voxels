#pragma once

#include "structure.hpp"

namespace gen {

class tree : public structure {
public:
	tree(superchunk* sc);
	~tree();

	bool can_place_at(int x, int y, int z);
	void place(int x, int y, int z);
};

}
