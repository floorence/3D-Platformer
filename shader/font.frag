#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D diffuse0;

void main() {
	vec4 texColor = texture(diffuse0, texCoord);
    if (texColor.r < 0.1) {
        discard;
    } else {
        texColor = vec4(vec3(texColor.r), 1.0);
    }
    FragColor = texColor;
}