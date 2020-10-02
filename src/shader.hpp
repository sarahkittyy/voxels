#pragma once

#include <GL/glew.h>

// gl shader
class shader {
public:
	// init with the vertex & fragment shader sources
	shader(const char* vs, const char* fs);

	// set opengl to use the shader
	void use();

	// delete the internal opengl program
	void del();

	// get the program id
	GLuint prog() const;

private:
	// initializes the shader
	void init(const char* vs, const char* fs);

	// default shaders
	static const char* default_vs;
	static const char* default_fs;

	// prog id
	GLuint m_prog;
};
