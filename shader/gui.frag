#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D diffuse0;
uniform vec3 color;
uniform vec4 tintColor;
uniform bool colorOverride;

void main() {
	vec3 result;
	if (colorOverride) {
		result = color;
	} else {
		result = vec3(texture(diffuse0, texCoord));
	}
	result = mix(result, tintColor.rgb, tintColor.a);
	FragColor = vec4(result, 1.0);
}