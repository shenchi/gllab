#include "Shader.hpp"
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
	if (GL_FALSE == result) {
		char buf[4096] = {0};
		glGetShaderInfoLog(shader, sizeof(buf), 0, buf);
		printf("compile error:\n%s\n", buf);
		return 0;
	}

	return new Shader(shader);
}

Shader* Shader::CreateFromFile(GLenum type, const char *filename) {
	char *source = 0;
	Shader *shader = 0;
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	source = new char[size + 1];
	source[size] = 0;

	long true_size = 0;
	if (1 != (true_size = fread(source, size, 1, fp)) ) {
		fclose(fp);
		delete [] source;
		return 0;
	}
	fclose(fp);

	shader = CreateFromSource(type, source);
	delete [] source;

	return shader;
}

GLenum Shader::getType() const {
	int type;
	glGetShaderiv(m_shader, GL_SHADER_TYPE, &type);
	return (GLenum)type;
}