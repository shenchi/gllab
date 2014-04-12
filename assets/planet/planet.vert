#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 vertNormal;
out vec2 uv;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

void main() {
	uv = texcoord;
	vertNormal = (matModel * vec4(normal, 1.0)).xyz;
	gl_Position = matProj * matView * matModel * vec4(position, 1.0);
}