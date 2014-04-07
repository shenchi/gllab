#version 400 core
in vec3 worldNormal;
in vec3 worldPos;

out vec4 fragData[4];

uniform LightAndMaterial {
	vec4 matColor;
	vec3 lightPos;
	vec4 lightAmbient;
	vec4 lightDiffuse;
	float lightIntense;
	float linearAtt;
	float quadAtt;
};

void main() {
	vec3 lightDir = lightPos - worldPos;
	float dist = length(lightDir);
	float dist_sqr = dist * dist;
	float intense = lightIntense / (1.0 + linearAtt * dist + quadAtt * dist_sqr);

	// fragColor = texColor * intense;
	fragData[0] = matColor * (lightAmbient + intense * lightDiffuse * clamp(dot(normalize(lightDir), worldNormal), 0.0, 1.0));
	float depth = 1.0 - pow(gl_FragCoord.z, 16);
	fragData[1] = vec4(depth, depth, depth, 1.0);
	fragData[2] = vec4(worldPos, 1.0);
	fragData[3] = vec4(worldNormal, 1.0);
}