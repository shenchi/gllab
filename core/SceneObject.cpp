#include "SceneObject.hpp"

SceneObject::SceneObject()
{
	m_scaling.x = 1.0f;
	m_scaling.y = 1.0f;
	m_scaling.z = 1.0f;
}

SceneObject::~SceneObject()
{
}

void SceneObject::removeChild( SceneObject* child )
{
	for( std::vector< SceneObject* >::iterator it = m_children.begin();
		it != m_children.end(); ++it )
	{
		if( *it == child )
		{
			m_children.erase( it );
			return;
		}
	}
}

void SceneObject::removeAllChildren()
{
	m_children.clear();
}

bool SceneObject::addComponent( Component* component )
{
	if( component->onAddToOwner( this ) )
	{
		component->setOwner( this );
		m_components.push_back( component );
		return true;
	}
	return false;
}

void SceneObject::removeComponent( Component* component )
{
	for( std::vector< Component* >::iterator it = m_components.begin();
		it != m_components.end(); ++it )
	{
		if( *it == component )
		{
			m_components.erase( it );
			return;
		}
	}
}

void SceneObject::removeAllComponents()
{
	m_components.clear();
}