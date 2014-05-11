#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 matM;
uniform mat4 matVP;

out vec4 worldPosition;
out vec4 worldNormal;

void main() {
	worldPosition = matM * vec4(position, 1.0);
	gl_Position = matVP * worldPosition;
	worldNormal = matM * vec4(normal, 1.0);
}