#ifndef ENGINE_H
#define ENGINE_H

#include "common.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "VertexBuffer.hpp"
#include "Program.hpp"

class Engine {
	Engine(const Engine &) {}

public:
	Engine() : m_window(0) {}
	~Engine();

	void run(int w, int h);

protected:
	virtual bool onInit() = 0;
	virtual void onFrame(float dt) = 0;
	virtual void onRelease() = 0;

	void useProgram(Program *program);

	Timer *m_timer;

private:
	GLFWwindow *m_window;
};

#endif

