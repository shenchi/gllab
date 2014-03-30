#ifndef ENGINE_H
#define ENGINE_H

#include "common.hpp"
#include "Camera.hpp"
#include "VertexBuffer.hpp"
#include "Program.hpp"

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

