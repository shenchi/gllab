#ifndef MESHFILTER_H
#define MESHFILTER_H

#include "Component.hpp"

class MeshFilter : public Component
{

public:
	MeshFilter() : Component( TYPE_MESHFILTER ), m_mesh( 0 ) {}

	virtual bool	onAddToOwner( SceneObject* owner );

};

#endif // MESHFILTER_H