#include "Program.h"

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