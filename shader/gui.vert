#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec2 aColor;

out vec2 texCoord;
out vec3 color; // unused in any corresponding fragment shaders, for now.

uniform mat4 projection;

void main() {
	texCoord = aTex;

	gl_Position = projection * vec4(aPos, 1.0);
}