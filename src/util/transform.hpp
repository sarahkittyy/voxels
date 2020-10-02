#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace util {

/**
 * @brief Transformation class to manage 3D space transforms.
 * 
 */
class transform {
public:
	transform();

	/**
	 * @brief Resets the transform back to the identity matrix.
	 * 
	 */
	void reset();

	/**
	 * @brief A copy of the identity matrix.
	 * 
	 */
	static const transform identity;

	/**
	 * @brief Convert the transform to a GLM matrix.
	 * 
	 * @return glm::mat4
	 */
	glm::mat4 to_mat4() const;

	/**
	 * @brief Set the transform's position
	 * 
	 * @param p 
	 */
	void set_pos(glm::vec3 p);
	/**
	 * @brief Get the transform's position
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 get_pos() const;
	/**
	 * @brief Translate the transform through space
	 * 
	 * @param p The displacement to move 
	 * @return transform& *this
	 */
	transform& move(glm::vec3 p);

	/**
	 * @brief Set the transform's rotation
	 * 
	 * @param angles Euler angles to rotate
	 */
	void set_rotation(glm::vec3 angles);
	/**
	 * @brief Get the transform's rotation
	 * 
	 * @return glm::vec3 Euler angles representing the current rotation
	 */
	glm::vec3 get_rotation() const;
	/**
	 * @brief Rotate by the given euler angle displacement.
	 * 
	 * @param angles Euler angles to rotate by
	 * @return transform& *this
	 */
	transform& rotate(glm::vec3 angles);

	/**
	 * @brief Set the transform's scale
	 * 
	 * @param s 
	 */
	void set_scale(glm::vec3 s);
	/**
	 * @brief Get the transform's scale
	 * 
	 * @return glm::vec3 
	 */
	glm::vec3 get_scale() const;
	/**
	 * @brief Scale the transform
	 * 
	 * @param s 
	 * @return transform& *this
	 */
	transform& scale(glm::vec3 s);

	/**
	 * @brief Combine two transforms.
	 * 
	 * @param other 
	 * @return transform The new transform
	 */
	transform operator*(const transform& other) const;
	/**
	 * @brief Combine another transform with this one
	 * 
	 * @param other 
	 * @return transform& *this
	 */
	transform& operator*=(const transform& other);

private:
	/// internal transformations to combine in to_mat4()
	glm::vec3 m_translation;
	glm::quat m_rotation;
	glm::vec3 m_scaling;
};

}
