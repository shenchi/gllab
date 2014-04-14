#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Component.hpp"

class VertexBuffer;

class Mesh {
public:
	Mesh() {}
	~Mesh();

	static Mesh* CreateFromFile(const char *filename);
	static Mesh* CreateScreenQuad();

	void render();

private:
	std::vector<VertexBuffer*> m_meshes;
};

#endif

