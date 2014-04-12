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

class PlanetScene : public Engine {

	Camera *m_cam;
	Material *m_mat;
	Mesh *m_sphere;

	unsigned int count;
public:
	virtual bool onInit() {
		m_cam = new Camera();
		m_cam->setPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10.0f);
		m_cam->setPosition(0.0f, 0.0f, 4.0f);
		m_cam->setLookAt(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		m_mat = Material::CreateMaterial(Program::CreateFromFile("../assets/planet/planet.vert", "../assets/planet/planet.frag"));

		// m_sphere = Mesh::CreateQuadXY();
		m_sphere = Mesh::CreateSphere(1.0f, 40, 40);

		m_mat->setUniform("matProj", m_cam->getMatProjection());
		m_mat->setUniform("matView", m_cam->getMatView());

		m_mat->setUniform("matModel", glm::mat4(1.0f));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		count = 0;
		return true;
	}

	virtual void onFrame() {
		count++;
		float r = count / 100.0f;

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, -1.0f, 0.0f));
		m_mat->setUniform("matModel", model);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_mat->bind();
		m_sphere->render();
	}

	virtual void onRelease() {
		delete m_sphere;
		delete m_mat;
		delete m_cam;
	}

};

int main() {
	PlanetScene e;
	e.run(800, 600);
	check_error();
	return 0;
}