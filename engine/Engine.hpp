#ifndef ENGINE_H
#define ENGINE_H

#include "core/common.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "core/VertexBuffer.hpp"
#include "core/Program.hpp"

class EngineBase {
	EngineBase(const EngineBase &) {}

public:
	EngineBase() : m_window(0) {}
	~EngineBase();

	void run(int w, int h);

protected:
	virtual bool onInit() = 0;
	virtual void onFrame(float dt) = 0;
	virtual void onRelease() = 0;

	Timer *m_timer;

private:
	GLFWwindow *m_window;
};

class Engine : public EngineBase {
// public:
private:
	virtual bool onInit();
	virtual void onFrame(float dt);
	virtual void onRelease();
};

#endif

