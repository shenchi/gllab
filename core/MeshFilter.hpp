#ifndef MESHFILTER_H
#define MESHFILTER_H

#include "Component.hpp"
#include "SceneObject.hpp"
#include "Mesh.hpp"

class MeshFilter : public Component
{

public:
	MeshFilter() : Component( TYPE_MESHFILTER ), m_mesh( 0 ) {}

	virtual bool	onAddToOwner( SceneObject* owner );

private:
	Mesh*			m_mesh;
};

#endif // MESHFILTER_H