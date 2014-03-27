#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Program.h"

class Engine {
	Engine(const Engine &) {}

public:
	Engine() : m_window(0) {}
	~Engine();

	void run();

protected:
	virtual bool onInit() = 0;
	virtual void onFrame() = 0;
	virtual void onRelease() = 0;

	void useProgram(Program *program);

private:
	GLFWwindow *m_window;
};

#endif

