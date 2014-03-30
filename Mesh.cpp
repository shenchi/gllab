#include "common.hpp"
#include "Mesh.hpp"
#include "VertexBuffer.hpp"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// #include <cstdio>

using namespace std;
using namespace Assimp;
using namespace glm;

Mesh* Mesh::CreateFromFile(const char *filename) {
	Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | /* aiProcess_GenSmoothNormals | */ aiProcess_FlipUVs);

	if (!scene)
		return 0;

	Mesh *mesh = new Mesh();
	mesh->m_meshes.resize(scene->mNumMeshes);

	for (size_t i = 0; i < mesh->m_meshes.size(); ++i) {
		const aiMesh* m = scene->mMeshes[i];
		bool hasNormal = m->HasNormals();

		unsigned int vertCount = m->mNumVertices;
		vector<unsigned int> indices;
		vector<vec3> positions;
		vector<vec3> normals;
		// vector<vec3> uvs;
		unsigned long posDataSize = (sizeof(float) * 3 * vertCount);

		vector<AttributeDesc> layout;
		layout.push_back(AttributeDesc(0, 3, GL_FLOAT, sizeof(float) * 3, 0));
		if (hasNormal) layout.push_back(AttributeDesc(1, 3, GL_FLOAT, sizeof(float) * 3, (void*)posDataSize));

		for (size_t j = 0; j < vertCount; ++j) {
			const aiVector3D *pos = &(m->mVertices[j]);
			positions.push_back(vec3(pos->x, pos->y, pos->z));
			// printf("pos: %f, %f, %f\n", pos->x, pos->y, pos->z);
			if (hasNormal) {
				const aiVector3D *norm = &(m->mNormals[j]);
				normals.push_back(vec3(norm->x, norm->y, norm->z));
				// printf("norm: %f, %f, %f\n", norm->x, norm->y, norm->z);
			}
		}

		VertexBuffer *vb = VertexBuffer::CreateVertexBuffer(layout.data(), layout.size(), vertCount, 0, indices.size(), indices.data());
		if (!vb) return 0;
		vb->setData(0, posDataSize, positions.data());
		if (hasNormal) vb->setData(posDataSize, posDataSize, normals.data());

		mesh->m_meshes[i] = vb;
	}

	// printf("%d mesh in total\n", mesh->m_meshes.size());

	return mesh;
}

void Mesh::render() {
	for (vector<VertexBuffer*>::iterator i = m_meshes.begin();
		i != m_meshes.end();
		++i) {
		if (*i) (*i)->render();
	}
}