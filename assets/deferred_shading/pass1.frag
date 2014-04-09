#version 400 core
in vec3 worldPosition;
in vec3 worldNormal;

uniform Material {
	vec4 diffuse;
	float specularIntensity;
	float specularPower;
};

layout(location = 0) out vec4 rt0;
layout(location = 1) out vec4 rt1;
// layout(location = 2) out vec4 rt2;

void main() {
	rt0 = vec4(diffuse.xyz, specularIntensity);
	rt1 = vec4(worldNormal, specularPower);
	// rt2 = vec4(worldPosition, 1.0);
	gl_FragDepth = gl_FragCoord.z;
}