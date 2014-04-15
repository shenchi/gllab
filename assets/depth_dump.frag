#version 400 core

in vec2 uv;

uniform sampler2D depthTex;

out vec4 fragColor;

void main() {
	float depth = texture(depthTex, uv).x;
	depth = pow(depth, 32);
	fragColor = vec4(depth, depth, depth, 1.0);
}