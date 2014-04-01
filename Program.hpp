#ifndef PROGRAM_H
#define PROGRAM_H

#include "common.hpp"
#include "Shader.hpp"

class Program {
	Program(const Program &) {}
	Program(Shader *vs, Shader *fs, GLuint program): m_program(program), m_vs(vs), m_fs(fs) {}
public:
	Program(): m_program(0), m_vs(0), m_fs(0) { }
	~Program();

	static Program* CreateFromFile(const char *vertFilename, const char *fragFilename);

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
