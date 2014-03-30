#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
// layout(location = 2) in vec2 texCoord;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform vec3 cameraPos;

out vec3 worldNormal;
out vec3 worldViewDir;
// out vec2 uv;

void main (void) {
	// uv = texCoord;

	worldNormal = (matModel * vec4(normal, 1.0)).xyz;
	vec4 worldPos = matModel * vec4(position, 1.0);

	worldViewDir = normalize(cameraPos - worldPos.xyz);

	gl_Position = matProj * matView * worldPos;
}