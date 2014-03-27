#ifndef SHADER_H
#define SHADER_H

#include "common.h"

class Shader {

	Shader(const Shader&) {}
	Shader():m_shader(0) {}
	Shader(GLuint shader):m_shader(shader) {}

public:

	~Shader();

	static Shader *CreateFromSource(GLenum type, const char *source);
	static Shader *CreateFromFile(GLenum type, const char* filename);

	GLuint getShader() const {return m_shader;}

	GLenum getType() const;

private:
	GLuint m_shader;
};

#endif
