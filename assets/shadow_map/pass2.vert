#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 worldPos;
out vec4 worldNormal;
out vec4 lightSSPos;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;
uniform mat4 lightViewProj;

void main() {
	worldPos = matModel * vec4(position, 1.0);
    worldNormal = matModel * vec4(normal, 0.0);
	gl_Position = matProj * matView * worldPos;
	lightSSPos = lightViewProj * worldPos;
}