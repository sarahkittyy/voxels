#include "util/transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace util {

transform const transform::identity = transform();

transform::transform() {
	reset();
}

void transform::reset() {
	m_translation = glm::vec3();
	m_rotation	  = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	m_scaling	  = glm::vec3(1.0f, 1.0f, 1.0f);
}

void transform::set_pos(glm::vec3 p) {
	m_translation = p;
}

glm::vec3 transform::get_pos() const {
	return m_translation;
}

transform& transform::move(glm::vec3 p) {
	m_translation += p;
	return *this;
}

void transform::set_rotation(glm::vec3 angles) {
	m_rotation = glm::quat(angles);
}

glm::vec3 transform::get_rotation() const {
	return glm::eulerAngles(m_rotation);
}

transform& transform::rotate(glm::vec3 angles) {
	m_rotation *= glm::quat(angles);
	return *this;
}

void transform::set_scale(glm::vec3 s) {
	m_scaling = s;
}

glm::vec3 transform::get_scale() const {
	return m_scaling;
}

transform& transform::scale(glm::vec3 s) {
	m_scaling *= s;
	return *this;
}

glm::mat4 transform::to_mat4() const {
	return glm::translate(glm::mat4(1.0f), m_translation) *
		   glm::toMat4(m_rotation) *
		   glm::scale(glm::mat4(1.0f), m_scaling);
}

transform transform::operator*(const transform& other) const {
	transform n;
	n.m_translation = m_translation + other.m_translation;
	n.m_rotation	= m_rotation * other.m_rotation;
	n.m_scaling		= m_scaling * other.m_scaling;
	return n;
}

transform& transform::operator*=(const transform& other) {
	m_translation += other.m_translation;
	m_rotation *= other.m_rotation;
	m_scaling *= other.m_scaling;
	return *this;
}

}
