#version 400 core
layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

void main() {
	gl_Position = matProj * matView * matModel * vec4(position, 1.0);
}