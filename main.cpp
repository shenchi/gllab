#include <cassert>
#include <cstdio>
#include <cmath>
#include "Engine.hpp"
#include "Mesh.hpp"
#include "FrameBuffer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void _check_error(const char* filename, int line) {
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Err %x at %s:%d\n", err, filename, line);
}

#define check_error() _check_error( __FILE__, __LINE__ )

class Lab : public Engine {

	Program *m_program;
	Camera m_camera;
	Mesh *m_mesh;
	FrameBuffer *m_framebuffer;

	GLuint locModel;
	GLuint locView;
	GLuint locProj;
	GLuint locPos;

	unsigned int count;
public:

	virtual bool onInit() {
		// m_program = Program::CreateFromFile("assets/diffuse_lighting.vert", "assets/diffuse_lighting.frag");
		// m_program = Program::CreateFromFile("assets/specular_lighting.vert", "assets/specular_lighting.frag");
		m_program = Program::CreateFromFile("assets/point_light.vert", "assets/point_light.frag");

		assert(m_program->getProgram());
		useProgram(m_program);

		locModel = m_program->getUniformLocation("matModel");
		locView = m_program->getUniformLocation("matView");
		locProj = m_program->getUniformLocation("matProj");
		// locPos = m_program->getUniformLocation("cameraPos");
		locPos = m_program->getUniformLocation("lightPos");

		m_camera.setPosition(0, 2, 2);
		m_camera.setRotation(-M_PI * 0.25f, 0.0f, 0.0f);
		m_camera.setPerspective(45.0f, 1.0f, 0.1f, 10.0f);

		glUniformMatrix4fv(locProj, 1, GL_FALSE, m_camera.getMatProjection());
		glUniformMatrix4fv(locView, 1, GL_FALSE, m_camera.getMatView());
		// glUniform3fv(locPos, 1, m_camera.getPosition());

		m_mesh = Mesh::CreateFromFile("assets/box.obj");
		assert(m_mesh != 0);

		m_framebuffer = FrameBuffer::CreateFrameBuffer(2048, 2048);
		assert(m_framebuffer);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());

		count = 0;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		return true;
	}

	virtual void onFrame() {
		count ++;
		float r = count / 100.0f;
		// m_camera.setRotation(-M_PI * 0.25f, sin(r) * M_PI * 0.1667f, 0.0f);
		// glUniformMatrix4fv(locView, 1, GL_FALSE, m_camera.getMatView());

		// glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, 1.0f, 0.0f));
		// model = glm::rotate(model, (float)M_PI * 0.2f * r, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));

		glm::vec3 lightPos = glm::vec3(0.7 * sin(r * 2), 0.7, 0.7 * cos(r * 2));
		glUniform3fv(locPos, 1, glm::value_ptr(lightPos));

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_mesh->render();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
		glBlitFramebuffer(0, 0, 2048, 2048, 0, 0, 2048, 2048, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	virtual void onRelease() {
		if (m_mesh) delete m_mesh;
		if (m_program) delete m_program;
		if (m_framebuffer) delete m_framebuffer;
	}
};

int main() {
	Lab lab;
	lab.run();
	check_error();
	return 0;
}
