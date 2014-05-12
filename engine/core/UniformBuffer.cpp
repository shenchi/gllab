#include "UniformBuffer.hpp"
#include <cstring>

UniformBuffer::~UniformBuffer() {
	if (!m_data) delete [] (char*)m_data;
	if (!m_ubo) glDeleteBuffers(1, &m_ubo);
}

UniformBuffer* UniformBuffer::CreateUniformBuffer(GLsizeiptr size, const void* data) {
	GLuint ubo = 0;

	glGenBuffers(1, &ubo);
	if (!ubo) return 0;

	UniformBuffer *ub = new UniformBuffer(ubo, size);
	ub->m_data = (void*) new char[size];
	if (data)
		memcpy(ub->m_data, data, size);
	else
		memset(ub->m_data, 0, size);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, ub->m_data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return ub;
}


void UniformBuffer::setData(GLintptr offset, GLsizeiptr size, const void* data) {
	if (offset + size > m_size) return;
	m_dirty = true;
	// printf("update data: [%lu, %ld]\n", offset, size);
	memcpy((void*)((char*)m_data + offset), data, size);
}

void UniformBuffer::bind(GLuint bindPoint) {
	if (m_dirty) {
		// printf("dirty! upload data!\n");
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, m_size, m_data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_dirty = false;
	}
	glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, m_ubo, 0, m_size);
}