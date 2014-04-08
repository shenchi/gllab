#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Object.hpp"
#include "Component.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>


class SceneObject : public Object
{


public:
	SceneObject();
	virtual ~SceneObject();


	void				setLayer( int layer ) { m_layer = layer; }
	int					getLayer() const { return m_layer; }
	void				setName( std::string& name ) { m_name = name; }
	const std::string&	getName() const { return m_name; }


	void				addChild( SceneObject* child ) { m_children.push_back( child ); }
	void				removeChild( SceneObject* child );
	void				removeAllChildren();

	std::vector< Component* >& getComponents() { return m_components; }
	bool				addComponent( Component* component );
	void				removeComponent( Component* component );
	void				removeAllComponents();


	void				setPosition( float x, float y, float z );
	void				setRotation( float x, float y, float z );
	void				setScaling( float x, float y, float z );
	const glm::vec3&	getPosition() const { return m_translation; }
	const glm::vec3&	getRotation() const { return m_rotation; }
	const glm::vec3&	getScaling() const { return m_scaling; }

protected:

	int								m_layer;
	std::string						m_name;
	std::vector< SceneObject* >		m_children;
	std::vector< Component* >		m_components;

	glm::vec3						m_translation;
	glm::vec3						m_rotation;
	glm::vec3						m_scaling;


};
#endif // SCENEOBJECT_H