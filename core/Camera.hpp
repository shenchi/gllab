#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera {
public:
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);

	void setLookAt(float x, float y, float z, float upX, float upY, float upZ);
	void setPerspective(float fov, float aspect, float zNear, float zFar);

	const float* getPosition() const;

	const glm::mat4& getMatView() const { return m_view; }
	const glm::mat4& getMatProjection() const { return m_proj; }

private:
	glm::mat4 m_view;
	glm::mat4 m_proj;
	//glm::mat4 m_mvp;

	glm::vec3 m_pos;
	glm::vec3 m_rot;

	void updateView();
};

#endif

