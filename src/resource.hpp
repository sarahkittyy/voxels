#pragma once

#include <string>
#include <unordered_map>

#include "texture.hpp"

// for managing intensive resources and preventing multiple unnecessary loads
class resource {
public:
	// construct
	resource();

	// retrieve a texture
	texture& tex(const std::string& path);

private:
	// all stored textures
	std::unordered_map<std::string, texture> m_texs;
};
