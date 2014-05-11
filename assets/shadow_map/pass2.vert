#version 400 core
layout(location = 0) in vec3 position;

out vec4 ssPos;
out vec4 lightSSPos;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;
uniform mat4 lightViewProj;

void main() {
	vec4 worldPos = matModel * vec4(position, 1.0);
	gl_Position = matProj * matView * worldPos;
	lightSSPos = lightViewProj * worldPos;
}