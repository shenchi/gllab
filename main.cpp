#include <cassert>
#include <cstdio>
#include <cmath>
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float canvas[] = {
	// front
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	// back
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	// left
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	// right
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	// top
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	// bottom
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
};

unsigned int canvas_indices[] = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10, 8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23,
};

const char *vert_shader = 
R"(#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

out vec3 worldNormal;
out vec2 uv;

void main (void) {
	uv = texCoord;

	worldNormal = (matModel * vec4(normal.xyz, 1.0)).xyz;

	gl_Position = matProj * matView * matModel * vec4(position, 1.0);
}
)";

const char *frag_shader = 
R"(#version 400 core
in vec3 worldNormal;
in vec2 uv;
out vec4 fragColor;

void main (void) {

	vec4 diff = vec4(0.4, 1.0, 0.4, 1.0);
	vec4 ambi = vec4(0.2, 0.2, 0.2, 1.0);
	vec3 light = normalize(vec3(1.0, 0.5, 1.0));

	vec4 lightColor = clamp(dot(light, worldNormal), 0.0, 1.0) * diff + ambi;

	float n = noise1(uv * 10.0) * 0.5 + 0.5;
	fragColor = vec4(n, n, n, 1.0) * lightColor;
}
)";

void _check_error(const char* filename, int line) {
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		printf("Err %x at %s:%d\n", err, filename, line);
}

#define check_error() _check_error( __FILE__, __LINE__ )

class Lab : public Engine {

	Program m_program;
	Camera m_camera;
	VertexBuffer *m_vb;

	GLuint locModel;
	GLuint locView;
	GLuint locProj;

	unsigned int count;
public:

	virtual bool onInit() {
		m_program.attachVertexShader(Shader::CreateFromSource(GL_VERTEX_SHADER, vert_shader));
		m_program.attachPixelShader(Shader::CreateFromSource(GL_FRAGMENT_SHADER, frag_shader));
		m_program.link();
		assert(m_program.getProgram());
		useProgram(&m_program);

		locModel = glGetUniformLocation(m_program.getProgram(), "matModel");
		locView = glGetUniformLocation(m_program.getProgram(), "matView");
		locProj = glGetUniformLocation(m_program.getProgram(), "matProj");

		m_camera.setPosition(0, 2, 2);
		m_camera.setRotation(-M_PI * 0.25f, 0.0f, 0.0f);
		m_camera.setPerspective(45.0f, 1.0f, 0.1f, 10.0f);

		glUniformMatrix4fv(locProj, 1, GL_FALSE, m_camera.getMatProjection());
		glUniformMatrix4fv(locView, 1, GL_FALSE, m_camera.getMatView());

		AttributeDesc layout[] = {
			{0, 3, GL_FLOAT, sizeof(float) * 8, 0},
			{1, 3, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float) * 3)},
			{2, 2, GL_FLOAT, sizeof(float) * 8, (void*)(sizeof(float) * 6)}
		};

		m_vb = VertexBuffer::CreateVertexBuffer(layout, 3, 24, canvas, 36, canvas_indices);
		if (!m_vb) return false;

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

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)M_PI * 0.1f * r, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_vb->render();
	}

	virtual void onRelease() {
		delete m_vb;
	}
};

int main() {
	Lab lab;
	lab.run();
	check_error();
	return 0;
}
