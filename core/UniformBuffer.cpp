#include "UniformBuffer.hpp"

UniformBuffer::~UniformBuffer() {
	if (!m_ubo) glDeleteBuffers(1, &m_ubo);
}


UniformBuffer* UniformBuffer::CreateUniformBuffer(GLsizeiptr size, const void* data) {
	GLuint ubo = 0;

	glGenBuffers(1, &ubo);
	if (!ubo) return 0;

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return new UniformBuffer(ubo, size);
}


void UniformBuffer::setData(GLintptr offset, GLsizeiptr size, const void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bind(GLuint bindPoint) {
	// glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, m_ubo);
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, m_ubo, 0, m_size);
}