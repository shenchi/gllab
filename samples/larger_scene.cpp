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
		m_cam->setPosition(70, 70, 70);
		m_cam->setLookAt(0, 0, 0, 0, 1, 0);
		m_cam->setPerspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);

		m_mat->setUniform("matVP", m_cam->getMatProjection() * m_cam->getMatView());
		m_mat->setUniform("matM", glm::mat4(1.0f));

		m_scene = Mesh::CreateFromFile("../assets/nature/nature.obj");
		// m_scene = Mesh::CreateFromFile("../assets/nature/3x3x3.obj");
		assert(m_scene != 0);

		m_ground = Mesh::CreateQuadXY(70.0f, true);
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

int main() {
	LargerScene scene;
	scene.run(800, 600);
	check_error();
	return 0;
}