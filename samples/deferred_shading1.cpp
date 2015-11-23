#include <cassert>
#include <cstdio>
#include <cmath>

#include <gllab.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void _check_error(const char* filename, int line) {
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Err %x at %s:%d\n", err, filename, line);
}

#define check_error() _check_error( __FILE__, __LINE__ )

#define USE_FOUR_VIEW

#ifdef USE_FOUR_VIEW
class FourView {
	Material *m_material;
	Mesh *m_mesh;
public:
	FourView(Texture *tex1, Texture *tex2, Texture *tex3, Texture *tex4): m_material(0), m_mesh(0) {
		m_mesh = Mesh::CreateQuadXY();
		m_material = Material::CreateMaterial(Program::CreateFromFile(
			"../assets/deferred_shading/four_view.vert",
			"../assets/deferred_shading/four_view.frag"));

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
#endif

class DeferredShading : public EngineBase {

	Program* m_pass1;
	Program* m_pass2;

	Mesh* m_quadMesh;
	Material *m_matLightPass;

	Mesh *m_mesh;
	Material *m_material1;
	Camera m_camera;
	FrameBuffer *m_gbuffer;

#ifdef USE_FOUR_VIEW
    FrameBuffer *m_finalBuffer;
    FourView *m_fourView;
#endif
public:

	virtual bool onInit() {
		//========================================
		// prepare material
		m_pass1 = Program::CreateFromFile("../assets/deferred_shading/pass1.vert", "../assets/deferred_shading/pass1.frag");
		m_pass2 = Program::CreateFromFile("../assets/deferred_shading/pass2.vert", "../assets/deferred_shading/pass2.frag");
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
		m_mesh = Mesh::CreateFromFile("../assets/box.obj");
		assert(m_mesh != 0);
		m_quadMesh = Mesh::CreateQuadXY();
		//========================================

		//========================================
		// prepare frame buffer
		RenderTargetDesc rts[] = {
			RenderTargetDesc(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE),
            RenderTargetDesc(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE)
		};
        
		m_gbuffer = FrameBuffer::CreateFrameBuffer(800, 600, 2, rts, true, true);
		assert(m_gbuffer);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gbuffer->getFrameBuffer());
		GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(2, drawbuffers);
		//========================================

#ifdef USE_FOUR_VIEW
        
        m_finalBuffer = FrameBuffer::CreateFrameBuffer(800, 600, 1, rts, false, false);
        
        m_fourView = new FourView(
            m_gbuffer->getColorTexture(0),
		 	m_gbuffer->getColorTexture(1),
		 	m_gbuffer->getDepthTexture(),
		 	m_finalBuffer->getColorTexture(0));
#endif

		result = result && m_matLightPass->setTexture("rt0", m_gbuffer->getColorTexture(0));
		result = result && m_matLightPass->setTexture("rt1", m_gbuffer->getColorTexture(1));
		result = result && m_matLightPass->setTexture("rt2", m_gbuffer->getDepthTexture());
		result = result && m_matLightPass->setUniform("invVP", m_camera.getInverseView() * m_camera.getInverseProjection());
		assert( result == true );

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

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		return true;
	}

	virtual void onFrame(float dt) {
		float r = m_timer->elapsed();

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

#ifdef USE_FOUR_VIEW
        glBindFramebuffer(GL_FRAMEBUFFER, m_finalBuffer->getFrameBuffer());
#else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
        glDisable(GL_DEPTH_TEST);

		m_matLightPass->bind();
		m_quadMesh->render();
        
#ifdef USE_FOUR_VIEW
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_fourView->render();
#endif
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
