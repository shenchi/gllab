#include "Program.hpp"


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

	return new Program(vs, fs, program);
}

Program::~Program() {
	if (m_program) glDeleteProgram(m_program);
	m_program = 0;
}

void Program::link() {
	if (!m_vs || !m_fs) return;

	if (!m_program) m_program = glCreateProgram();
	if (!m_program) return;
	glAttachShader(m_program, m_vs->getShader());
	glAttachShader(m_program, m_fs->getShader());
	glLinkProgram(m_program);
}

GLuint Program::getProgram() const {
	int result = GL_FALSE;
	if (!m_program) return 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &result);
	if (GL_FALSE == result) return 0;
	return m_program;
}

GLuint Program::getAttributeLocation(const char* name) const {
	if (!getProgram()) return 0;
	return glGetAttribLocation(m_program, name);
}

GLuint Program::getUniformLocation(const char* name) const {
	if (!getProgram()) return 0;
	return glGetUniformLocation(m_program, name);
}

GLuint Program::getUniformBlockIndex(const char* name) const {
	if (!getProgram()) return 0;
	return glGetUniformBlockIndex(m_program, name);
}