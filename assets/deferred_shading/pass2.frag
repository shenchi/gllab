#version 400 core

in vec2 uv;

uniform sampler2D rt0;
uniform sampler2D rt1;
uniform sampler2D rt2;
uniform mat4 invVP;

uniform Lights {
	int lightNum;
	vec4 lightPos[20];
	vec4 lightDiffuse[20];
};

out vec4 fragColor;

void main() {
	vec4 matDiffuse = texture(rt0, uv);
	matDiffuse.w = 1.0;
	vec3 worldNormal = texture(rt1, uv).xyz;
	float depth = texture(rt2, uv).x;

	vec4 worldPosition = (invVP * vec4(2 * uv.x - 1, 2 * uv.y - 1, 2 * depth - 1, 1.0));
	worldPosition /= worldPosition.w;

	vec4 finalColor = vec4(0.0);
	for (int i = 0; i < lightNum; i++) {
		vec3 lightDir = lightPos[i].xyz - worldPosition.xyz;
		float dist = length(lightDir);
		// if (dist > 0.4) continue;
		lightDir = normalize(lightDir);
		float intensity = 100.0 / (1.0 + 1.0 * dist + 100.0 * dist * dist);
		finalColor = finalColor + intensity * lightDiffuse[i] * matDiffuse * clamp(dot(lightDir, worldNormal), 0.0, 1.0);
	}

	fragColor = finalColor;
	// fragColor = vec4(2 * uv.x - 1, 2 * uv.y - 1, 2 * depth - 1, 1.0);
}