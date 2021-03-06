#include "MeshFilter.hpp"

bool MeshFilter::onAddToOwner( SceneObject* owner )
{
	std::vector< Component* >& components = owner->getComponents();
	for( int i = 0; i < components.size(); ++i )
	{
		if( components[i]->getType() == Component::TYPE_MESHFILTER ) // There's already a mesh attached to this SO.
			return false;
	}
	return true;
}