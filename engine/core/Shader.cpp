#include "Shader.hpp"
#include <cstdio>

static const char* builtInVertShaderHeader = 
R"(#version 400
layout(location = 0) vec4 inPosition;
layout(location = 1) vec4 inNormal;
layout(location = 2) vec2 inUV;

uniform block BuiltInCamera {
	mat4 matProjection;
	mat4 matView;
};

uniform block BuiltInTransform {
	mat4 matModel;
};

mat4 matProjectionView;
mat4 matProjectionViewModel;
)";


static const char* builtInVertShaderMain = 
R"(void main() {
	matProjectionView = matProjection * matView;
	matProjectionViewModel = matProjectionView * matModel;
	gl_Position = vert();
}
)";

Shader::~Shader() {
	if (m_shader) glDeleteShader(m_shader);
	m_shader = 0;
}

const char* Shader::shaderTypeName(GLenum type) {
	switch (type) {
	case GL_VERTEX_SHADER:
		return "vertex shader";
	case GL_FRAGMENT_SHADER:
		return "fragment shader";
	}
	return "unknow type shader";
}

Shader* Shader::CreateFromSource(GLenum type, const char *source, bool withBuiltIn) {
	GLuint shader = 0;

	shader = glCreateShader(type);
	if (!shader) return 0;

	if (withBuiltIn && type == GL_VERTEX_SHADER) {
		const char* sources[3] = {builtInVertShaderHeader, source, builtInVertShaderMain};
		glShaderSource(shader, 3, sources, 0);
	} else {
		glShaderSource(shader, 1, &source, 0);
	}

	glCompileShader(shader);

	int result = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		char buf[4096] = {0};
		glGetShaderInfoLog(shader, sizeof(buf), 0, buf);
		printf("%s compile error:\n%s\n", shaderTypeName(type), buf);
		return 0;
	}

	return new Shader(shader, withBuiltIn);
}

Shader* Shader::CreateFromFile(GLenum type, const char *filename, bool withBuiltIn) {
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

	shader = CreateFromSource(type, source, withBuiltIn);
	delete [] source;

	return shader;
}

GLenum Shader::getType() const {
	int type;
	glGetShaderiv(m_shader, GL_SHADER_TYPE, &type);
	return (GLenum)type;
}