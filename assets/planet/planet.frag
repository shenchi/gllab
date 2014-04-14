#version 400 core
in vec3 vertNormal;
in vec2 uv;

out vec4 fragColor;

void main() {
	// float gray = mod(floor(10.0 * uv.x) + floor(10.0 * uv.y), 2.0);
	float land = noise1(8.0 * uv);
	land += 0.5 * noise1(16.0 * uv);
	land = smoothstep(0.05, 0.1, land);
	land += float(land > 0.0) * 0.2 * noise1(80.0 * uv);
	fragColor = vec4(0.0, land, 1.0 - land, 1.0);
}