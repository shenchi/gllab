#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "Component.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"

class MeshRenderer : public Component
{
public:
	MeshRenderer() : Component( TYPE_MESHRENDERER ), m_meshFilter( 0 ) {}

	virtual bool	onAddToOwner( SceneObject* owner );

private:

	MeshFilter*						m_meshFilter; // Mesh to render

	std::vector< Material* >		m_materials;

};

#endif // MESHRENDERER_H