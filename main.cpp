#include <cassert>
#include <cstdio>
#include <cmath>

#include "gllab.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void _check_error(const char* filename, int line) {
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Err %x at %s:%d\n", err, filename, line);
}

#define check_error() _check_error( __FILE__, __LINE__ )

class Lab : public Engine {

	Material *m_material;
	Camera m_camera;
	Mesh *m_mesh;
	FrameBuffer *m_framebuffer;

	unsigned int count;
public:

	virtual bool onInit() {
		//========================================
		// prepare material
		// m_program = Program::CreateFromFile("assets/diffuse_lighting.vert", "assets/diffuse_lighting.frag");
		// m_program = Program::CreateFromFile("assets/specular_lighting.vert", "assets/specular_lighting.frag");
		m_material = Material::CreateMaterial(Program::CreateFromFile("assets/point_light.vert", "assets/point_light.frag"));

		const Program *m_program = m_material->getProgram();

		m_camera.setPosition(0, 2, 2);
		m_camera.setRotation(-M_PI * 0.25f, 0.0f, 0.0f);
		m_camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);

		m_material->setUniform("matProj", m_camera.getMatProjection());
		m_material->setUniform("matView", m_camera.getMatView());

		m_material->setUniform("matColor", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
		m_material->setUniform("lightAmbient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		m_material->setUniform("lightDiffuse", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
		m_material->setUniform("lightIntense", 5.0f);
		m_material->setUniform("linearAtt", 1.0f);
		m_material->setUniform("quadAtt", 4.0f);
		//========================================

		//========================================
		// prepare mesh
		m_mesh = Mesh::CreateFromFile("assets/box.obj");
		assert(m_mesh != 0);
		//========================================

		//========================================
		// prepare frame buffer
		RenderTargetDesc rts[] = {
			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
			RenderTargetDesc(true),
			RenderTargetDesc(GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT)
		};
		m_framebuffer = FrameBuffer::CreateFrameBuffer(1600, 1200, 4, rts);
		assert(m_framebuffer);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
		GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawbuffers);
		//========================================

		count = 0;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		return true;
	}

	virtual void onFrame() {
		count ++;
		float r = count / 100.0f;

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, -1.0f, 0.0f));
		m_material->setUniform("matModel", model);

		glm::vec4 lightPos = glm::vec4(0.7 * sin(r * 2), 0.7f, 0.7 * cos(r * 2), 0.0f);
		m_material->setUniform("lightPos", lightPos);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_material->bind();
		m_mesh->render();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, 1600, 1200, 0, 600, 800, 1200, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, 1600, 1200, 800, 600, 1600, 1200, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glBlitFramebuffer(0, 0, 1600, 1200, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glReadBuffer(GL_COLOR_ATTACHMENT3);
		glBlitFramebuffer(0, 0, 1600, 1200, 800, 0, 1600, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	virtual void onRelease() {
		if (m_mesh) delete m_mesh;
		if (m_material) delete m_material;
		if (m_framebuffer) delete m_framebuffer;
	}
};

int main() {
	Lab lab;
	lab.run(800, 600);
	check_error();
	return 0;
}
