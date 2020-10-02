#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "color.hpp"

enum class WRAP_MODE {
	REPEAT			= GL_REPEAT,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	CLAMP_TO_EDGE	= GL_CLAMP_TO_EDGE,
	CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum class FILTER_MODE {
	NEAREST = GL_NEAREST,
	LINEAR	= GL_LINEAR,
};

// texture loading abstraction
class texture {
public:
	texture();

	// load texture from file
	texture(const char* path);

	// load texture from file
	void load_from_file(const char* path);

	// set the gl filtering mode
	void set_filter_mode(FILTER_MODE m);

	// set the texture wrapping mode
	void set_wrap_mode(WRAP_MODE m);

	// in the case of GL_CLAMP_TO_BORDER, set the border color
	void set_border_color(color bc);

	// delete the internal gl representation
	void del();

	// bind the texture as active
	void bind();

	// get the texture's size
	glm::vec2 get_size() const;

private:
	// internal gl texture
	GLuint m_tex;

	// image width & height
	int m_w;
	int m_h;

	// has the texture been loaded?
	bool m_initd;
};
