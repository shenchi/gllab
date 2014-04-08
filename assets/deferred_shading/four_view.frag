#version 400 core

in vec2 uv;

uniform sampler2D view1;
uniform sampler2D view2;
uniform sampler2D view3;
uniform sampler2D view4;

out vec4 fragColor;

void main() {
	vec4 texColor = vec4(1.0, 0.0, 0.0, 1.0);
	if (uv.x < 0.5) {
		if (uv.y < 0.5) {
			// left-bottom
			float depth = texture(view3, uv * 2).x;
			depth = pow(depth, 10);
			texColor = vec4(depth, depth, depth, 1.0);
		} else {
			// left-top
			texColor = texture(view1, (uv - vec2(0.0, 0.5)) * 2);
		}
	} else {
		if (uv.y < 0.5) {
			// right-bottom
			texColor = texture(view4, (uv - vec2(0.5, 0.0)) * 2);
		} else {
			// right-top
			texColor = texture(view2, (uv - vec2(0.5, 0.5)) * 2);
		}
	}

	fragColor = texColor;
	// fragColor = texture(view2, uv);
}