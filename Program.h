#ifndef PROGRAM_H
#define PROGRAM_H

#include "common.h"
#include "Shader.h"

class Program {
	Program(const Program &) {}
public:
	Program(): m_program(0), m_vs(0), m_fs(0) { }
	~Program();

	void attachVertexShader(Shader *shader) {m_vs = shader; }
	void attachPixelShader(Shader *shader) {m_fs = shader; }
	void link();

	GLuint getProgram() const;

	GLuint getAttributeLocation(const char* name) const;
	GLuint getUniformLocation(const char* name) const;

private:
	GLuint m_program;

	Shader *m_vs, *m_fs;
};

#endif
