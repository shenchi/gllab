#include <cassert>

#include <gllab.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void _check_error(const char* filename, int line) {
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Err %x at %s:%d\n", err, filename, line);
}

#define check_error() _check_error( __FILE__, __LINE__ )

#define USE_SHADOW

#ifndef USE_SHADOW

class LargerScene : public EngineBase {

	Camera* m_cam;

	Mesh* m_scene;
	Mesh* m_ground;
	Material* m_mat;

	float rot;

public:
	virtual bool onInit() {
		m_mat = Material::CreateMaterial(Program::CreateFromFile("../assets/nature/nature.vert", "../assets/nature/nature.frag"));
		assert(m_mat != 0);

		m_cam = new Camera();
		m_cam->setPosition(100, 100, 100);
		m_cam->setLookAt(0, 0, 0, 0, 1, 0);
		m_cam->setPerspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);

		m_mat->setUniform("matVP", m_cam->getMatProjection() * m_cam->getMatView());
		m_mat->setUniform("matM", glm::mat4(1.0f));

		m_scene = Mesh::CreateFromFile("../assets/nature/nature.obj");
		// m_scene = Mesh::CreateFromFile("../assets/nature/3x3x3.obj");
		assert(m_scene != 0);

		m_ground = Mesh::CreateQuadXY(140.0f, true);
		assert(m_ground != 0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		rot = 0.0f;

		return true;
	}

	virtual void onFrame(float dt) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rot += dt;
		m_mat->setUniform("matM", glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, -1.0f, 0.0f)));

		m_mat->bind();
		m_scene->render();

		m_mat->setUniform("matM", glm::rotate(glm::mat4(1.0f), 3.1415926f * 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f)));
		m_ground->render();
	}

	virtual void onRelease() {
		delete m_cam;
		delete m_mat;
		delete m_scene;
	}
};

#else

class ShadowMapPass {
    Material *m_mat;
    FrameBuffer *m_fb;
    Camera *m_cam;
    
    size_t m_bufferSize;
public:
    ShadowMapPass(size_t bufferSize) : m_mat(0), m_fb(0), m_cam(0), m_bufferSize(bufferSize) {
        m_mat = Material::CreateMaterial(Program::CreateFromFile("../assets/shadow_map/pass1.vert", "../assets/shadow_map/pass1.frag"));
        RenderTargetDesc rts[] = {
            RenderTargetDesc()
        };
        m_fb = FrameBuffer::CreateFrameBuffer(bufferSize, bufferSize, 0, rts, true, true);
        m_cam = new Camera();
    }
    
    void setLight(glm::vec3 pos, glm::vec3 dir) {
        m_cam->setPosition(pos.x, pos.y, pos.z);
        m_cam->setLookAt(dir.x, dir.y, dir.z, 0.0f, 1.0f, 0.0f);
        m_cam->setPerspective(30.0f, 1.0f, 0.1f, 20.0f);
        
        assert( m_mat->setUniform("matView", m_cam->getMatView()) == true );
        assert( m_mat->setUniform("matProj", m_cam->getMatProjection()) == true );
    }
    
    glm::mat4 getViewProjMatrix() {
        return m_cam->getMatProjection() * m_cam->getMatView();
    }
    
    void setModelMatrix(const glm::mat4& mat) {
        assert( m_mat->setUniform("matModel", mat) == true );
    }
    
    void bind() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fb->getFrameBuffer());
        // GLuint attachs[] = {GL_COLOR_ATTACHMENT0};
        // glDrawBuffers(1, attachs);
        glViewport(0, 0, m_bufferSize, m_bufferSize);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_mat->bind();
    }
    
    void unbind() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // glDrawBuffer(GL_BACK);
        glViewport(0, 0, 800, 600);
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

class LargerScene : public EngineBase {
    
    ShadowMapPass *m_shadowMapPass;
    
    Camera *m_cam;
    Material *m_mat;
    Mesh* m_scene;
    Mesh *m_ground;
    glm::mat4 m_sceneMat;
    glm::mat4 m_groundMat;
    
    // Material *m_testMat;
    // Mesh *m_testMesh;
public:
    virtual bool onInit() {
        glm::vec3 lightPos(5.0f, 5.0f, 5.0f);
        glm::vec3 lightDist(0.0f, 0.0f, 0.0f);
        
        m_shadowMapPass = new ShadowMapPass(2048);
        m_shadowMapPass->setLight(lightPos, lightDist);
        
        // m_testMat = Material::CreateMaterial(Program::CreateFromFile("../assets/quad.vert", "../assets/depth_dump.frag"));
        // m_testMesh = Mesh::CreateQuadXY();
        
        // assert( m_testMat->setTexture("depthTex", m_shadowMapPass->getTexture()) == true );
        
        m_cam = new Camera();
        m_cam->setPosition(0, 3, 3);
        m_cam->setLookAt(0, 0, 0, 0, 1, 0);
        m_cam->setPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10.0f);
        m_mat = Material::CreateMaterial(Program::CreateFromFile("../assets/shadow_map/pass2.vert", "../assets/shadow_map/pass2.frag"));
        assert( m_mat->setUniform("matProj", m_cam->getMatProjection()) == true );
        assert( m_mat->setUniform("matView", m_cam->getMatView()) == true );
        assert( m_mat->setUniform("lightViewProj", m_shadowMapPass->getViewProjMatrix()) == true );
        assert( m_mat->setUniform("lightPos", glm::vec4(lightPos, 1.0f)) == true );
        assert( m_mat->setUniform("lightDir", glm::vec4(glm::normalize(lightPos - lightDist), 0.0f)) == true );
        assert( m_mat->setUniform("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) == true );
        assert( m_mat->setUniform("color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)) == true);
        // assert( m_mat->setUniform("invViewProj", glm::inverse(m_cam->getMatProjection() * m_cam->getMatView())) == true);
        assert( m_mat->setTexture("depthTex", m_shadowMapPass->getTexture()) == true );
        
        m_scene = Mesh::CreateFromFile("../assets/nature/nature.obj");
        m_sceneMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.04f, 0.04f, 0.04f));
        m_ground = Mesh::CreateQuadXY(10, true);
        m_groundMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
        m_groundMat = glm::rotate(m_groundMat, (float)M_PI * 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f));
        
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        return true;
    }
    
    virtual void onFrame(float dt) {
        m_sceneMat = glm::rotate(m_sceneMat, dt * 0.1f, glm::vec3(0, 1, 0));
        
        m_shadowMapPass->bind();
        m_shadowMapPass->setModelMatrix(m_groundMat);
        m_ground->render();
        m_shadowMapPass->setModelMatrix(m_sceneMat);
        m_scene->render();
        
        m_shadowMapPass->unbind();
        // glDisable(GL_DEPTH_TEST);
        // m_testMat->bind();
        // m_testMesh->render();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_mat->bind();
        m_mat->setUniform("matModel", m_groundMat);
        m_ground->render();
        m_mat->setUniform("matModel", m_sceneMat);
        m_scene->render();
    }
    
    virtual void onRelease() {
        delete m_shadowMapPass;
        
        delete m_cam;
        delete m_mat;
        delete m_scene;
        delete m_ground;
        // delete m_testMat;
        // delete m_testMesh;
    }
    
};

#endif

int main() {
	LargerScene scene;
	scene.run(800, 600);
	check_error();
	return 0;
}