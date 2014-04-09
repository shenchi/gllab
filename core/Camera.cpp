#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void Camera::setPosition(float x, float y, float z) {
	m_pos = vec3(x, y, z);
	updateView();
}

void Camera::setRotation(float x, float y, float z) {
	m_rot = vec3(x, y, z);
	updateView();
}

void Camera::setLookAt(float x, float y, float z, float upX, float upY, float upZ) {
	m_view = lookAt(m_pos, vec3(x, y, z), vec3(upX, upY, upZ));
}

const float* Camera::getPosition() const {
	return value_ptr(m_pos);
}

void Camera::updateView() {
	m_view = mat4(1.0f);

	// order zx
	m_view = rotate(m_view, m_rot.z, vec3(0.0f, 0.0f, 1.0f));
	m_view = rotate(m_view, m_rot.x, vec3(-1.0f, 0.0f, 0.0f));
	m_view = rotate(m_view, m_rot.y, vec3(0.0f, 1.0f, 0.0f));

	m_view = translate(m_view, -m_pos);
}


void Camera::setPerspective(float fov, float aspect, float zNear, float zFar) {
	m_proj = perspective(fov, aspect, zNear, zFar);
}


const glm::mat4 Camera::getInverseView() const {
	return inverse(m_view);
}

const glm::mat4 Camera::getInverseProjection() const {
	return inverse(m_proj);
}