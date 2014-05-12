#include "Program.hpp"
#include <cstdio>

Program* Program::CreateFromFile(const char *vertFilename, const char *fragFilename) {
	Shader *vs = Shader::CreateFromFile(GL_VERTEX_SHADER, vertFilename);
	if (!vs) return 0;
	Shader *fs = Shader::CreateFromFile(GL_FRAGMENT_SHADER, fragFilename);
	if (!fs) {
		delete vs;
		return 0;
	}

	GLuint program = glCreateProgram();
	if (!program) {
		delete vs;
		delete fs;
		return 0;
	}

	glAttachShader(program, vs->getShader());
	glAttachShader(program, fs->getShader());
	glLinkProgram(program);

	int result = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (GL_FALSE == result) {
		glDeleteProgram(program);
		delete vs;
		delete fs;
		return 0;
	}

	Program* ret = new Program(vs, fs, program);
	ret->reflect();
	return ret;
}

Program::~Program() {
	if (m_program) glDeleteProgram(m_program);
	m_program = 0;
	if (m_vs) delete m_vs;
	if (m_fs) delete m_fs;
}

// void Program::link() {
// 	if (!m_vs || !m_fs) return;

// 	if (!m_program) m_program = glCreateProgram();
// 	if (!m_program) return;
// 	glAttachShader(m_program, m_vs->getShader());
// 	glAttachShader(m_program, m_fs->getShader());
// 	glLinkProgram(m_program);
// }


void Program::reflect() {
	m_attributes.clear();
	m_uniforms.clear();
	m_uniformBlocks.clear();

	if (!getProgram()) return;

	char name[1024] = {0};
	int length = 0, size = 0;
	GLenum type;

	int nAttrib = 0;
	glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &nAttrib);
	for (int i = 0; i < nAttrib; ++i) {
		glGetActiveAttrib(m_program, i, sizeof(name), &length, &size, &type, name);
		m_attributes[name] = i;
	}

	int nUniforms = 0;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &nUniforms);
	for (GLuint i = 0; i < nUniforms; ++i) {
		int idx = 0;
		glGetActiveUniform(m_program, i, sizeof(name), &length, &size, &type, name);
		glGetActiveUniformsiv(m_program, 1, &i, GL_UNIFORM_BLOCK_INDEX, &idx);
		if (idx < 0) {
			GLint loc = glGetUniformLocation(m_program, name);
			m_uniforms[name] = UniformDesc{ idx, loc, size, type };
		} else {
			GLint offset = 0;
			glGetActiveUniformsiv(m_program, 1, &i, GL_UNIFORM_OFFSET, &offset);
			m_uniforms[name] = UniformDesc{ idx, offset, size, type };
		}
	}

	int nBlocks = 0;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &nBlocks);
	for (GLuint i = 0; i < nBlocks; ++i) {
		glGetActiveUniformBlockName(m_program, i, sizeof(name), &length, name);
		glGetActiveUniformBlockiv(m_program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
		m_uniformBlocks.push_back( std::make_pair(name, size) );
	}
}

GLuint Program::getProgram() const {
	int result = GL_FALSE;
	if (!m_program) return 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	if (GL_FALSE == result) return 0;
	return m_program;
}

GLint Program::getAttributeLocation(const std::string& name) const {
	if (!getProgram()) return -1;
	if (m_attributes.find(name) == m_attributes.cend()) return -1;
	return m_attributes.at(name);
}

const UniformDesc* Program::getUniform(const std::string& name) const {
	if (!getProgram()) return 0;
	if (m_uniforms.find(name) == m_uniforms.cend()) return 0;
	return &(m_uniforms.at(name));
}
