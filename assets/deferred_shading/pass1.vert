#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

out vec3 worldPosition;
out vec3 worldNormal;

void main() {
	worldPosition = (matModel * vec4(position, 1.0)).xyz;
	worldNormal = (matModel * vec4(normal, 1.0)).xyz;
	gl_Position = matProj * matView * matModel * vec4(position, 1.0);
}
