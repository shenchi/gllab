#version 400

in vec4 worldPosition;
in vec4 worldNormal;

out vec4 fragColor;

void main() {
	vec4 _ = worldPosition; // unused
	vec3 lightDir = normalize(vec3(1, 1, 1));

	float NdL = dot(normalize(worldNormal.xyz), lightDir);
	fragColor = vec4(0.2, 0.2, 0.2, 1.0) + clamp(NdL, 0, 1) * vec4(0.5, 0.5, 0.5, 0.0);
}