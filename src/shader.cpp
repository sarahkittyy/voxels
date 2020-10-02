#include "shader.hpp"

#include <stdexcept>
#include <string>

shader::shader(const char* vs, const char* fs) {
	init(vs, fs);
}

void shader::use() {
	glUseProgram(m_prog);
}

void shader::del() {
	glDeleteProgram(m_prog);
}

GLuint shader::prog() const {
	return m_prog;
}

void shader::init(const char* vs, const char* fs) {

	GLuint vsS, fsS;
	char infolog[512];
	int success;

	vsS = glCreateShader(GL_VERTEX_SHADER);
	fsS = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsS, 1, &vs, nullptr);
	glShaderSource(fsS, 1, &fs, nullptr);

	glCompileShader(vsS);
	glGetShaderiv(vsS, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsS, 512, NULL, infolog);
		throw std::runtime_error("Could not compile vertex shader: " + std::string(infolog));
	}

	glCompileShader(fsS);
	glGetShaderiv(fsS, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsS, 512, NULL, infolog);
		throw std::runtime_error("Could not compile fragment shader: " + std::string(infolog));
	}

	m_prog = glCreateProgram();
	glAttachShader(m_prog, vsS);
	glAttachShader(m_prog, fsS);

	glLinkProgram(m_prog);
	glGetProgramiv(m_prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_prog, 512, NULL, infolog);
		throw std::runtime_error("Could not link program: " + std::string(infolog));
	}

	glDeleteShader(vsS);
	glDeleteShader(fsS);
}

