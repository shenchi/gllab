#include "VertexBuffer.h"


VertexBuffer::~VertexBuffer() {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_vbo);
	if (m_ibo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_ibo);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);
}

size_t VertexBuffer::sizeOfType(GLenum type) {
	switch(type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_HALF_FLOAT:
		return 2;
	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FLOAT:
	case GL_FIXED:
	case GL_INT_2_10_10_10_REV:
	case GL_UNSIGNED_INT_2_10_10_10_REV:
		return 4;
	case GL_DOUBLE:
		return 8;
	}
	return 0;
}

VertexBuffer *VertexBuffer::CreateVertexBuffer(AttributeDesc *layout, int numAttributes, int numVertices, const void* data, int numIndices, const void* idxData) {
	int vertexSize = 0;
	for(int i = 0; i < numAttributes; ++i)
		vertexSize += layout[i].components * sizeOfType(layout[i].type);

	int totalSize = vertexSize * numVertices;

	GLuint vao = 0, vbo = 0, ibo = 0;
	glGenVertexArrays(1, &vao);
	if (!vao) return 0;

	bool failure = false;
	glBindVertexArray(vao);

	while(1) {
		glGenBuffers(1, &vbo);
		if (!vbo) {
			failure = true;
			break;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, totalSize, data, GL_STATIC_DRAW);

		if (numIndices > 0) {
			glGenBuffers(1, &ibo);
			if (!ibo) {
				glDeleteBuffers(1, &vbo);
				failure = true;
				break;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), idxData, GL_STATIC_DRAW);
		}

		for (int i = 0; i < numAttributes; ++i) {
			glEnableVertexAttribArray(layout[i].location);
			glVertexAttribPointer(layout[i].location, 
				layout[i].components,
				layout[i].type,
				GL_FALSE,
				layout[i].stride,
				layout[i].offset);
		}

		break;
	}

	glBindVertexArray(0);
	if (failure) {
		glDeleteVertexArrays(1, &vao);
		return 0;
	}

	return new VertexBuffer(vao, vbo, ibo, numVertices, numIndices);
}


void VertexBuffer::render() {
	if (!m_vao) return;
	glBindVertexArray(m_vao);

	if (m_ibo)
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	glBindVertexArray(0);
}