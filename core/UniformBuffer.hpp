#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP

#include "common.hpp"

class UniformBuffer {

	UniformBuffer(const UniformBuffer&) {}
	UniformBuffer(GLuint ubo, GLsizeiptr size): m_ubo(ubo), m_size(size) {}
public:
	~UniformBuffer();

	static UniformBuffer* CreateUniformBuffer(GLsizeiptr size, const void* data = 0);

	void setData(GLintptr offset, GLsizeiptr size, const void* data);

	GLuint getUniformBuffer() const { return m_ubo; }
	void bind(GLuint index);

private:
	GLuint m_ubo;
	GLsizeiptr m_size;
};

#endif 

