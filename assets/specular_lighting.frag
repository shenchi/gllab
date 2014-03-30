#version 400 core

in vec3 worldNormal;
in vec3 worldViewDir;
//in vec2 uv;
out vec4 fragColor;

void main() {

	vec4 spec = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 ambi = vec4(0.2, 0.2, 0.2, 1.0);
	vec3 light = normalize(vec3(0.0, 1.0, -1.0));

	vec3 refl = 2 * worldNormal * dot(worldNormal, light) - light;
	float specFactor = clamp(dot(refl, worldViewDir), 0.0, 1.0);
	specFactor = pow(specFactor, 32);
	vec4 lightColor = specFactor * spec * 5.0f + ambi;

	// float n = noise1(uv * 10.0) * 0.5 + 0.5;
	// vec4 texColor = vec4(n, n, n, 1.0);
	vec4 texColor = vec4(0.7, 0.7, 0.7, 1.0);
	fragColor = texColor * lightColor;
}