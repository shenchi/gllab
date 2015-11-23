#version 400 core

in vec4 worldPos;
in vec4 worldNormal;
in vec4 lightSSPos;

uniform vec4 lightPos;
uniform vec4 lightDir;
uniform vec4 lightColor;
uniform vec4 color;
uniform sampler2D depthTex;

// uniform mat4 invViewProj;
uniform mat4 matProj;
uniform mat4 matView;
out vec4 fragColor;

void main() {
	vec3 lightTexPos = lightSSPos.xyz / lightSSPos.w;
	vec2 uv = (lightTexPos.xy + 1) * 0.5;
	float depth = texture(depthTex, uv).x;
	float depth2 = (lightTexPos.z + 1) * 0.5;
    
    vec4 delta = lightPos - worldPos;
    float dist = length(delta);
    float LdotL = dot(normalize(lightDir), normalize(delta));
    float NdotL = dot(normalize(worldNormal), normalize(delta));
    vec4 lightedColor = NdotL * pow(LdotL, 40.0) * color * lightColor;// / dist;
    
	if (depth < depth2 - 0.00005)
		fragColor = vec4(0.3, 0.3, 0.3, 1) * lightedColor;
	else
		fragColor = lightedColor;
}