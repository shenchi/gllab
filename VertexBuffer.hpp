#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "common.hpp"

struct AttributeDesc {
	int location;
	int components;
	GLenum type;
	int stride;
	void* offset;

	AttributeDesc(int _location, int _components, GLenum _type, int _stride, void* _offset):
		location(_location),
		components(_components),
		type(_type),
		stride(_stride),
		offset(_offset) {}
};

class VertexBuffer {

	VertexBuffer(const VertexBuffer &) {}
	VertexBuffer(GLuint vao, GLuint vbo, GLuint ibo, int vertexCount, int indexCount)
		: m_vao(vao), m_vbo(vbo), m_ibo(ibo), m_vertexCount(vertexCount), m_indexCount(indexCount) {}
public:

	~VertexBuffer();

	static size_t sizeOfType(GLenum type);
	
	static VertexBuffer *CreateVertexBuffer(AttributeDesc *layout, int numAttributes, 
		int numVertices, const void* data = 0, int numIndices = 0, const void* idxData = 0);


	void setData(GLintptr offset, GLsizeiptr size, const void* data);

	void render();

private:
	GLuint m_vao, m_vbo, m_ibo;

	int m_vertexCount;
	int m_indexCount;
};

#endif
