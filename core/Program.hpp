#ifndef PROGRAM_H
#define PROGRAM_H

#include "common.hpp"
#include "Shader.hpp"
#include <unordered_map>
#include <vector>
#include <string>

struct UniformDesc {
	GLint blockIndex; // -1 for default uniform block
	GLint location; // location for default uniform block, offset for named uniform block
	int size;
	GLenum type;
};

class Program {
	Program(const Program &) {}
	Program(Shader *vs, Shader *fs, GLuint program): m_program(program), m_vs(vs), m_fs(fs) {}
public:
	Program(): m_program(0), m_vs(0), m_fs(0) { }
	~Program();

	static Program* CreateFromFile(const char *vertFilename, const char *fragFilename);

	void attachVertexShader(Shader *shader) {m_vs = shader; }
	void attachPixelShader(Shader *shader) {m_fs = shader; }
	// void link();  disable relink

	GLuint getProgram() const;

	GLint getAttributeLocation(const std::string& name) const;
	const UniformDesc* getUniform(const std::string& name) const;

	typedef std::pair< std::string, int > BlockDesc;
	const std::vector< BlockDesc >& getUniformBlockList() const { return m_uniformBlocks; }

private:
	GLuint m_program;

	Shader *m_vs, *m_fs;

	void reflect();
	std::unordered_map< std::string, GLint > m_attributes;
	std::unordered_map< std::string, UniformDesc > m_uniforms;

	std::vector< BlockDesc > m_uniformBlocks;
};

#endif
