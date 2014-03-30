#include "Engine.hpp"

Engine::~Engine() {
}

void Engine::run() {
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(512, 512, "GLLab", NULL, NULL);

	if (!m_window) {
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	
	if (!onInit()) {
		glfwTerminate();
		return;
	}

	while (!glfwWindowShouldClose(m_window)) {
		onFrame();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	onRelease();

	glfwTerminate();
}

void Engine::useProgram(Program *program) {
	if (!program || !program->getProgram()) return;
	glUseProgram(program->getProgram());
}