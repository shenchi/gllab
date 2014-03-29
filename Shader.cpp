#include "Shader.h"
#include <cstdio>

Shader::~Shader() {
	if (m_shader) glDeleteShader(m_shader);
	m_shader = 0;
}

Shader* Shader::CreateFromSource(GLenum type, const char *source) {
	GLuint shader = 0;

	shader = glCreateShader(type);
	if (!shader) return 0;

	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
		return 0;

	return new Shader(shader);
}

Shader* Shader::CreateFromFile(GLenum type, const char *filename) {
	char *source = 0;
	FILE *fp = fopen(filename, "r");
	if (!fp) return 0;

	fseek(fp, 0, SEEK_END); // HERE

	fclose(fp);
	return CreateFromSource(type, source);
}

GLenum Shader::getType() const {
	int type;
	glGetShaderiv(m_shader, GL_SHADER_TYPE, &type);
	return (GLenum)type;
}