#include "core/common.hpp"
#include "Mesh.hpp"
#include "core/VertexBuffer.hpp"
#include "SceneObject.hpp"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cmath>

// #include <cstdio>

using namespace std;
using namespace Assimp;
using namespace glm;

Mesh::~Mesh() {
	for (vector<VertexBuffer*>::iterator i = m_meshes.begin();
		i != m_meshes.end();
		++i) {
		if (*i) delete (*i);
	}
}

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
		unsigned int faceCount = m->mNumFaces;
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

		for (size_t i = 0; i < faceCount; ++i) {
			const aiFace* face = &(m->mFaces[i]);
			if (face->mNumIndices == 3) {
				indices.push_back(face->mIndices[0]);
				indices.push_back(face->mIndices[1]);
				indices.push_back(face->mIndices[2]);
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

Mesh* Mesh::CreateQuadXY(float scale, bool normal) {
	float screenQuads[] = {
		-scale, -scale,  0.0f,  0.0f,  0.0f,
		 scale, -scale,  0.0f,  1.0f,  0.0f,
		 scale,  scale,  0.0f,  1.0f,  1.0f,
		-scale,  scale,  0.0f,  0.0f,  1.0f,
	};

	float screenQuadsN[] = {
		-scale, -scale,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 scale, -scale,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 scale,  scale,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-scale,  scale,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	};

	unsigned int indices[] = {0, 1, 2, 0, 2, 3};

	AttributeDesc layout[] = {
		{0, 3, GL_FLOAT, sizeof(float) * 5, 0},
		{1, 2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3)}
	};

	AttributeDesc layoutN[] = {
		{0, 3, GL_FLOAT, sizeof(float) * 8, 0},
		{1, 3, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float) * 3)},
		{2, 2, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float) * 6)}
	};

	VertexBuffer *vb = VertexBuffer::CreateVertexBuffer(normal ? layoutN : layout, 3, 4, normal ? screenQuadsN : screenQuads, 6, indices);
	if (!vb) return 0;

	Mesh* mesh = new Mesh();
	mesh->m_meshes.push_back(vb);

	return mesh;
}

Mesh* Mesh::CreateBox(float scale) {
	// float vertices[] = {

	// };
	// unsigned int indices[] = {};
	return 0;
}

Mesh* Mesh::CreateSphere(float radius, int longitudeSlice, int latitudeSlice) {
	vector<unsigned int> indices;
	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> texcoords;

	float longitudeStep = 2 * M_PI / longitudeSlice;
	float latitudeStep = M_PI / latitudeSlice;
	float uStep = 1.0f / longitudeSlice;
	float vStep = 1.0f / latitudeSlice;

	for (int j = 0; j <= latitudeSlice; ++j) {
		float y = -cos(j * latitudeStep) * radius;
		float r = sin(j * latitudeStep) * radius;
		float v = vStep * j;

		for (int i = 0; i <= longitudeSlice; ++i) {
			float x = -cos(i * longitudeStep) * r;
			float z = sin(i * longitudeStep) * r;
			float u = uStep * i;

			positions.push_back(vec3(x, y, z));
			normals.push_back(normalize(vec3(x, y, z)));
			texcoords.push_back(vec2(u, v));
		}
	}

	for (int j = 0; j < latitudeSlice; ++j) {
		for (int i = 0; i < longitudeSlice; ++i) {
			int k = j * (longitudeSlice + 1) + i;

			indices.push_back(k);
			indices.push_back(k + 1);
			indices.push_back(k + longitudeSlice + 2);

			indices.push_back(k);
			indices.push_back(k + longitudeSlice + 2);
			indices.push_back(k + longitudeSlice + 1);
		}
	}

	size_t size0 = sizeof(float) * 3 * positions.size();
	size_t offset1 = size0;
	size_t size1 = sizeof(float) * 3 * normals.size();
	size_t offset2 = offset1 + size1;
	size_t size2 = sizeof(float) * 2 * texcoords.size();

	AttributeDesc layout[] = {
		{0, 3, GL_FLOAT, sizeof(float) * 3, 0},
		{1, 3, GL_FLOAT, sizeof(float) * 3, (void*)offset1},
		{2, 2, GL_FLOAT, sizeof(float) * 2, (void*)offset2}
	};

	VertexBuffer *vb = VertexBuffer::CreateVertexBuffer(layout, 3, positions.size(), 0, indices.size(), indices.data());
	if (!vb) return 0;
	vb->setData(0, size0, positions.data());
	vb->setData(offset1, size1, normals.data());
	vb->setData(offset2, size2, texcoords.data());

	Mesh* mesh = new Mesh();
	mesh->m_meshes.push_back(vb);

	return mesh;
}

void Mesh::render() {
	for (vector<VertexBuffer*>::iterator i = m_meshes.begin();
		i != m_meshes.end();
		++i) {
		if (*i) (*i)->render();
	}
}