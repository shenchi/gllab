#ifndef COMPONENT_H
#define COMPONENT_H

#include "Object.hpp"

class SceneObject;

class Component : public Object
{
public:
	enum Type
	{
		TYPE_UNKNOWN = -1,
		TYPE_MESH,
	};

public:
	Component( int type ) : m_type( type ), m_owner( 0 ) {}
	virtual ~Component() {}


	int				getType() const { return m_type; }

	SceneObject*	getOwner() { return m_owner; }
	void			setOwner( SceneObject* owner ) { m_owner = owner; }


	virtual bool	onAddToOwner( SceneObject* owner ) { return true; }
protected:
	int				m_type;

	SceneObject*	m_owner;
};


#endif // COMPONENT_H