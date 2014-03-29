#version 400 core
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