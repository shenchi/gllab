#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Component.hpp"

class VertexBuffer;

class Mesh : public Component {
public:
	Mesh() : Component( TYPE_MESH ) {}
	~Mesh();

	static Mesh* CreateFromFile(const char *filename);
	static Mesh* CreateScreenQuad();

	void render();


	virtual bool	onAddToOwner( SceneObject* owner );
private:
	std::vector<VertexBuffer*> m_meshes;
};

#endif

