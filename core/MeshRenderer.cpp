#include "MeshRenderer.hpp"

bool MeshRenderer::onAddToOwner( SceneObject* owner )
{
	std::vector< Component* > components = owner->getComponents();
	for( int i = 0; i < components.size(); ++i )
	{
		
	}
	return true;
}