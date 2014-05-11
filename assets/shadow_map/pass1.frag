#version 400 core

out vec4 fragColor;

void main() {
	// fragColor = gl_FragCoord;

	gl_FragDepth = gl_FragCoord.z;// / gl_FragCoord.w;
}