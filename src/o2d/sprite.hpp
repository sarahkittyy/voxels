#pragma once

#include <string>

#include "mesh.hpp"
#include "texture.hpp"
#include "util/transform.hpp"
#include "window.hpp"

namespace o2d {

// generic sprite class to render a 2d sprite :D
class sprite {
public:
	sprite(window& win);
	virtual ~sprite();

	void load_texture(const std::string& path);
	texture& get_texture();

	// draw the sprite
	void draw();

	// get the sprite's size
	glm::vec2 size() const;

	// get the internal transform
	util::transform& transform();

private:
	// main app window
	window& m_win;

	// internally stored texture
	texture* m_tex;

	// internal mesh
	mesh m_mesh;
	void generate_mesh();

	// internal transform
	util::transform m_transform;
};

}
