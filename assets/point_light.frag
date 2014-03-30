#version 400 core
in vec3 worldNormal;
in vec3 worldPos;
//in vec2 uv;

out vec4 fragColor;

uniform vec3 lightPos;

void main() {
	float dist = length(lightPos - worldPos);
	float dist_sqr = dist * dist;
	float intense = 2 / (1.0 + 1.0 * dist + 4.0 * dist_sqr);

	// float n = noise1(uv * 10.0) * 0.5 + 0.5;
	// vec4 texColor = vec4(n, n, n, 1.0);
	vec4 texColor = vec4(0.7, 0.7, 0.7, 1.0);

	fragColor = texColor * intense;
}