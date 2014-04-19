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

class ShadowMapPass {
	Material *m_mat;
	FrameBuffer *m_fb;
	Camera *m_cam;

	size_t m_bufferSize;
public:
	ShadowMapPass(size_t bufferSize) : m_bufferSize(bufferSize), m_mat(0), m_fb(0), m_cam(0) {
		m_mat = Material::CreateMaterial(Program::CreateFromFile("../assets/shadow_map/pass1.vert", "../assets/shadow_map/pass1.frag"));
		m_fb = FrameBuffer::CreateFrameBuffer(bufferSize, bufferSize, 0, 0, true, true);
		m_cam = new Camera();
	}

	void setLight(glm::vec3 pos, glm::vec3 dir) {
		m_cam->setPosition(pos.x, pos.y, pos.z);
		m_cam->setLookAt(dir.x, dir.y, dir.z, 0.0f, 1.0f, 0.0f);
		m_cam->setPerspective(20.0, 1.0, 0.1, 10.0);

		assert( m_mat->setUniform("matView", m_cam->getMatView()) == true );
		assert( m_mat->setUniform("matProj", m_cam->getMatProjection()) == true );
	}

	void setModelMatrix(const glm::mat4& mat) {
		assert( m_mat->setUniform("matModel", mat) == true );
	}

	void bind() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fb->getFrameBuffer());

		glViewport(0, 0, m_bufferSize, m_bufferSize);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_mat->bind();
	}

	Texture *getTexture() {
		return m_fb->getDepthTexture();
	}

	~ShadowMapPass() {
		if (m_mat) delete m_mat;
		if (m_fb) delete m_fb;
		if (m_cam) delete m_cam;
	}
};

class ShadowMap : public Engine {

	ShadowMapPass *m_shadowMapPass;
	Mesh *m_box;
	Mesh *m_ground;
	glm::mat4 m_groundMat;

	Material *m_testMat;
	Mesh *m_testMesh;
public:
	virtual bool onInit() {
		m_shadowMapPass = new ShadowMapPass(512);
		m_shadowMapPass->setLight(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_testMat = Material::CreateMaterial(Program::CreateFromFile("../assets/quad.vert", "../assets/depth_dump.frag"));
		m_testMesh = Mesh::CreateQuadXY();

		assert( m_testMat->setTexture("depthTex", m_shadowMapPass->getTexture()) == true );

		m_box = Mesh::CreateFromFile("../assets/box.obj");
		m_ground = Mesh::CreateQuadXY(10);
		m_groundMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
		m_groundMat = glm::rotate(m_groundMat, (float)M_PI * 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		return true;
	}

	virtual void onFrame(float dt) {
		m_shadowMapPass->bind();
		m_shadowMapPass->setModelMatrix(m_groundMat);
		m_ground->render();
		m_shadowMapPass->setModelMatrix(glm::mat4(1.0f));
		m_box->render();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, 1600, 1200);
		m_testMat->bind();
		m_testMesh->render();
	}

	virtual void onRelease() {
		delete m_shadowMapPass;

		delete m_testMat;
		delete m_testMesh;
	}

};

int main() {
	ShadowMap e;
	e.run(800, 600);
	check_error();
	return 0;
}