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
	
};

class ShadowMap : public Engine {

public:
	virtual bool onInit() {

	}

	virtual void onFrame() {

	}

	virtual void onRelease() {

	}

};

int main() {
	ShadowMap e;
	e.run(800, 600);
	check_error();
	return 0;
}