#ifndef SCENE_H
#define SCENE_H

#include "SceneObject.hpp"
#include "Camera.hpp"

class Scene : public Object
{

public:
	Scene() : m_curCamera( 0 ) {}

protected:
	SceneObject		m_root;

	Camera*			m_curCamera;
};

#endif // SCENE_H