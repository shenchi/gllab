#include "Engine.hpp"

EngineBase::~EngineBase() {
}

void EngineBase::run(int w, int h) {
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(w, h, "GLLab", NULL, NULL);

	if (!m_window) {
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	
	m_timer = new Timer();

	if (!onInit()) {
		glfwTerminate();
		return;
	}

	glfwSwapInterval(0); // turn off vsync

	float lastTime = m_timer->elapsed();
	float nowTime = 0.0f;
	int frameCount = 0;
	float delta = 0.0f;
	float totalDelta = 0.0f;

	while (!glfwWindowShouldClose(m_window)) {
		nowTime = m_timer->elapsed();
		delta = nowTime - lastTime;
		lastTime = nowTime;

		onFrame(delta);
		glfwSwapBuffers(m_window);

		frameCount++;
		totalDelta += delta;
		if (totalDelta > 1.0f) {
			totalDelta -= 1.0f;
			printf("frame rate: %d\n", frameCount);
			frameCount = 0;
		}

		glfwPollEvents();
	}

	onRelease();
	delete m_timer;
	glfwTerminate();
}


bool Engine::onInit() {
	return false;
}

void Engine::onFrame(float dt) {
	
}

void Engine::onRelease() {
	
}