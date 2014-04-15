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

	static Mesh* CreateQuadXY(float scale = 1.0f);
	static Mesh* CreateBox(float scale = 1.0f);
	static Mesh* CreateSphere(float radius = 1.0f, int longitudeSlice = 10, int latitudeSlice = 10);

	void render();

private:
	std::vector<VertexBuffer*> m_meshes;
};

#endif

