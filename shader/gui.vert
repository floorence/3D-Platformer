#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 texCoord;

uniform mat4 projection;

void main() {
	texCoord = aTex;

	gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}