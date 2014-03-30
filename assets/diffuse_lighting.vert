#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 texCoord;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;

out vec3 worldNormal;
//out vec2 uv;

void main (void) {
	//uv = texCoord;

	worldNormal = (matModel * vec4(normal.xyz, 1.0)).xyz;

	gl_Position = matProj * matView * matModel * vec4(position, 1.0);
}