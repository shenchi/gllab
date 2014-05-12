#ifndef SHADER_H
#define SHADER_H

#include "common.hpp"

class Shader {

	Shader(const Shader&) {}
	Shader():m_shader(0), m_hasBuiltIn(false) {}
	Shader(GLuint shader, bool withBuiltIn):m_shader(shader), m_hasBuiltIn(withBuiltIn) {}

public:

	~Shader();

	static Shader *CreateFromSource(GLenum type, const char *source, bool withBuiltIn = false);
	static Shader *CreateFromFile(GLenum type, const char* filename, bool withBuiltIn = false);

	GLuint getShader() const { return m_shader; }

	GLenum getType() const;

	bool hasBuiltInCodes() const { return m_hasBuiltIn; }

private:
	GLuint m_shader;
	bool m_hasBuiltIn;

	static const char* shaderTypeName(GLenum type);
};

#endif
