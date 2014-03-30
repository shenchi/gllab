#ifndef MESH_H
#define MESH_H

#include <vector>

class VertexBuffer;

class Mesh {
public:

	static Mesh* CreateFromFile(const char *filename);

	void render();
private:
	std::vector<VertexBuffer*> m_meshes;
};

#endif

