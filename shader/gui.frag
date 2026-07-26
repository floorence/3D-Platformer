#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D diffuse0;
uniform vec3 color;
uniform bool colorOverride;

void main() {
	if (colorOverride) {
		FragColor = vec4(color, 1.0);
	} else {
		FragColor = texture(diffuse0, texCoord);
	}
}