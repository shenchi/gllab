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

// class Lab : public Engine {

// 	Material *m_material;
// 	Camera m_camera;
// 	Mesh *m_mesh;
// 	FrameBuffer *m_framebuffer;

// 	unsigned int count;
// public:

// 	virtual bool onInit() {
// 		//========================================
// 		// prepare material
// 		// m_program = Program::CreateFromFile("assets/diffuse_lighting.vert", "assets/diffuse_lighting.frag");
// 		// m_program = Program::CreateFromFile("assets/specular_lighting.vert", "assets/specular_lighting.frag");
// 		m_material = Material::CreateMaterial(Program::CreateFromFile("assets/point_light.vert", "assets/point_light.frag"));

// 		const Program *m_program = m_material->getProgram();

// 		m_camera.setPosition(0, 2, 2);
// 		m_camera.setRotation(-M_PI * 0.25f, 0.0f, 0.0f);
// 		m_camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);

// 		m_material->setUniform("matProj", m_camera.getMatProjection());
// 		m_material->setUniform("matView", m_camera.getMatView());

// 		m_material->setUniform("matColor", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
// 		m_material->setUniform("lightAmbient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
// 		m_material->setUniform("lightDiffuse", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
// 		m_material->setUniform("lightIntense", 5.0f);
// 		m_material->setUniform("linearAtt", 1.0f);
// 		m_material->setUniform("quadAtt", 4.0f);
// 		//========================================

// 		//========================================
// 		// prepare mesh
// 		m_mesh = Mesh::CreateFromFile("assets/box.obj");
// 		assert(m_mesh != 0);
// 		//========================================

// 		//========================================
// 		// prepare frame buffer
// 		RenderTargetDesc rts[] = {
// 			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
// 			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
// 			RenderTargetDesc(true),
// 			RenderTargetDesc(GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT)
// 		};
// 		m_framebuffer = FrameBuffer::CreateFrameBuffer(1600, 1200, 4, rts);
// 		assert(m_framebuffer);

// 		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
// 		GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
// 		glDrawBuffers(4, drawbuffers);
// 		//========================================

// 		count = 0;

// 		glEnable(GL_CULL_FACE);
// 		glCullFace(GL_BACK);
// 		return true;
// 	}

// 	virtual void onFrame() {
// 		count ++;
// 		float r = count / 100.0f;

// 		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, -1.0f, 0.0f));
// 		m_material->setUniform("matModel", model);

// 		glm::vec4 lightPos = glm::vec4(0.7 * sin(r * 2), 0.7f, 0.7 * cos(r * 2), 0.0f);
// 		m_material->setUniform("lightPos", lightPos);

// 		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		m_material->bind();
// 		m_mesh->render();

// 		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
// 		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer->getFrameBuffer());
// 		glReadBuffer(GL_COLOR_ATTACHMENT0);
// 		glBlitFramebuffer(0, 0, 1600, 1200, 0, 600, 800, 1200, GL_COLOR_BUFFER_BIT, GL_LINEAR);
// 		glReadBuffer(GL_COLOR_ATTACHMENT1);
// 		glBlitFramebuffer(0, 0, 1600, 1200, 800, 600, 1600, 1200, GL_COLOR_BUFFER_BIT, GL_LINEAR);
// 		glReadBuffer(GL_COLOR_ATTACHMENT2);
// 		glBlitFramebuffer(0, 0, 1600, 1200, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);
// 		glReadBuffer(GL_COLOR_ATTACHMENT3);
// 		glBlitFramebuffer(0, 0, 1600, 1200, 800, 0, 1600, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);
// 		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
// 	}

// 	virtual void onRelease() {
// 		if (m_mesh) delete m_mesh;
// 		if (m_material) delete m_material;
// 		if (m_framebuffer) delete m_framebuffer;
// 	}
// };

class FourView {
	Material *m_material;
	Mesh *m_mesh;
public:
	FourView(Texture *tex1, Texture *tex2, Texture *tex3, Texture *tex4): m_material(0), m_mesh(0) {
		m_mesh = Mesh::CreateScreenQuad();
		m_material = Material::CreateMaterial(Program::CreateFromFile(
			"assets/deferred_shading/four_view.vert",
			"assets/deferred_shading/four_view.frag"));

		bool result = true;
		result = result && m_material->setTexture("view1", tex1);
		result = result && m_material->setTexture("view2", tex2);
		result = result && m_material->setTexture("view3", tex3);
		result = result && m_material->setTexture("view4", tex4);
		assert(result == true);
	}

	void render() {
		m_material->bind();
		m_mesh->render();
	}

	~FourView() {
		if (m_mesh) delete m_mesh;
		if (m_material) delete m_material;
	}
};

class DeferredShading : public Engine {

	Program* m_pass1;
	Program* m_pass2;

	Mesh* m_quadMesh;
	Material *m_matLightPass;

	Mesh *m_mesh;
	Material *m_material1;
	Camera m_camera;
	FrameBuffer *m_gbuffer;

	// FourView *m_fourView;

	unsigned int count;
public:

	virtual bool onInit() {
		//========================================
		// prepare material
		m_pass1 = Program::CreateFromFile("assets/deferred_shading/pass1.vert", "assets/deferred_shading/pass1.frag");
		m_pass2 = Program::CreateFromFile("assets/deferred_shading/pass2.vert", "assets/deferred_shading/pass2.frag");
		assert(m_pass1 != 0);
		assert(m_pass2 != 0);

		m_matLightPass = Material::CreateMaterial(m_pass2);
		m_material1 = Material::CreateMaterial(m_pass1);

		m_camera.setPosition(0, 2, 2);
		m_camera.setRotation(-M_PI * 0.25f, 0.0f, 0.0f);
		m_camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 10.0f);

		bool result = true;
		result = result && m_material1->setUniform("matProj", m_camera.getMatProjection());
		result = result && m_material1->setUniform("matView", m_camera.getMatView());

		result = result && m_material1->setUniform("diffuse", glm::vec4(0.3f, 0.3f, 0.7f, 1.0f));
		result = result && m_material1->setUniform("specularIntensity", 1.0f);
		result = result && m_material1->setUniform("specularPower", 1.0f);
		assert(result == true);

		//========================================

		//========================================
		// prepare mesh
		m_mesh = Mesh::CreateFromFile("assets/box.obj");
		assert(m_mesh != 0);
		m_quadMesh = Mesh::CreateScreenQuad();
		//========================================

		//========================================
		// prepare frame buffer
		RenderTargetDesc rts[] = {
			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
			RenderTargetDesc(GL_RGBA32F, GL_RGBA, GL_FLOAT),
			// RenderTargetDesc(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT)
		};
		m_gbuffer = FrameBuffer::CreateFrameBuffer(1600, 1200, 3, rts, true, true);
		assert(m_gbuffer);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gbuffer->getFrameBuffer());
		GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, drawbuffers);
		//========================================

		// m_fourView = new FourView(
		// 	m_gbuffer->getColorTexture(0),
		// 	m_gbuffer->getColorTexture(1),
		// 	m_gbuffer->getDepthTexture(),
		// 	m_gbuffer->getColorTexture(0));

		m_matLightPass->setTexture("rt0", m_gbuffer->getColorTexture(0));
		m_matLightPass->setTexture("rt1", m_gbuffer->getColorTexture(1));
		m_matLightPass->setTexture("rt2", m_gbuffer->getColorTexture(2));

		assert( m_matLightPass->setUniform("lightNum", 6) == true );
		glm::vec4 lightDiff[6] = {
			glm::vec4(1.0, 0.0, 0.0, 1.0),
			glm::vec4(0.0, 1.0, 0.0, 1.0),
			glm::vec4(0.0, 0.0, 1.0, 1.0),
			glm::vec4(1.0, 1.0, 0.0, 1.0),
			glm::vec4(1.0, 0.0, 1.0, 1.0),
			glm::vec4(0.0, 1.0, 1.0, 1.0),
		};
		assert(sizeof(lightDiff) == 4 * 4 * 6);
		assert( m_matLightPass->setUniform("lightDiffuse[0]", sizeof(lightDiff), &lightDiff) == true );

		count = 0;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		return true;
	}

	virtual void onFrame() {
		count ++;
		float r = count / 100.0f;

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, -1.0f, 0.0f));
		assert( m_material1->setUniform("matModel", model) == true );

		glm::vec4 lightPos[6];
		float sinr = sin(r * 2);
		float cosr = cos(r * 2);
		lightPos[0] = glm::vec4(1.0f * sinr,  0.3f, -1.0f * cosr, 0.0f);
		lightPos[1] = glm::vec4(1.0f * sinr, -0.3f,  1.0f * cosr, 0.0f);
		lightPos[2] = glm::vec4(1.0f * sinr, -1.0f * cosr,  0.3f, 0.0f);
		lightPos[3] = glm::vec4(1.0f * sinr,  1.0f * cosr, -0.3f, 0.0f);
		lightPos[4] = glm::vec4( 0.3f, -1.0f * cosr, 1.0f * sinr, 0.0f);
		lightPos[5] = glm::vec4(-0.3f,  1.0f * cosr, 1.0f * sinr, 0.0f);
		m_matLightPass->setUniform("lightPos[0]", sizeof(lightPos), &lightPos);
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_gbuffer->getFrameBuffer());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		m_material1->bind();
		m_mesh->render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		m_matLightPass->bind();
		m_quadMesh->render();
	}

	virtual void onRelease() {
		// if (m_fourView) delete m_fourView;

		if (m_mesh) delete m_mesh;
		if (m_material1) delete m_material1;

		if (m_quadMesh) delete m_quadMesh;
		if (m_matLightPass) delete m_matLightPass;

		if (m_pass1) delete m_pass1;
		if (m_pass2) delete m_pass2;
		if (m_gbuffer) delete m_gbuffer;
	}
};

int main() {
	// Lab lab;
	// lab.run(800, 600);
	DeferredShading ds;
	ds.run(800, 600);
	check_error();
	return 0;
}
