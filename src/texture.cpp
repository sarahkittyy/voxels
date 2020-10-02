#include "texture.hpp"

#include <stb_image.h>
#include <stdexcept>

texture::texture()
	: m_w(0),
	  m_h(0),
	  m_initd(false) {
}

texture::texture(const char* path)
	: m_w(0),
	  m_h(0),
	  m_initd(false) {
	load_from_file(path);
}

void texture::load_from_file(const char* path) {
	if (m_initd) {
		del();
	}

	m_initd = true;
	glGenTextures(1, &m_tex);

	int w, h, nc;
	unsigned char* data = stbi_load(path, &w, &h, &nc, 0);

	if (!data) {
		throw std::runtime_error("Could not load texture.");
	}

	bind();

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		nc == 4 ? GL_RGBA : GL_RGB,
		w,
		h,
		0,
		nc == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	m_w = w;
	m_h = h;
}

void texture::set_filter_mode(FILTER_MODE m) {
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)m);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)m);
}

void texture::set_border_color(color bc) {
	bind();

	float c[] = { bc.r, bc.g, bc.b, bc.a };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, c);
}

void texture::set_wrap_mode(WRAP_MODE m) {
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)m);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)m);
}

void texture::del() {
	if (!m_initd) throw std::runtime_error("Texture not initalized!");
	glDeleteTextures(1, &m_tex);
}

void texture::bind() {
	if (!m_initd) throw std::runtime_error("Texture not initalized!");
	glBindTexture(GL_TEXTURE_2D, m_tex);
}

glm::vec2 texture::get_size() const {
	return glm::vec2(m_w, m_h);
}
