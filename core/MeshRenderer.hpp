#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Component.hpp"
#include "Material.hpp"

class MeshRenderer : public Component
{
public:
	MeshRenderer() : Component( TYPE_MESHRENDERER ), m_mesh( 0 ) {}

	virtual bool	onAddToOwner( SceneObject* owner );

private:

	Mesh*							m_mesh; // Mesh to render

	std::vector< Material* >		m_materials;

};

#endif // MESHRENDERER_H