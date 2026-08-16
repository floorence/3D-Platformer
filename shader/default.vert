#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // not necessarily normalized
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aColor;

out vec3 crntPos;
out vec3 normal;
out vec2 texCoord;
out vec3 color;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
	crntPos = vec3(model * vec4(aPos, 1.0f)); // final position of vertex in world
	normal = aNormal;
	texCoord = aTex;
	color = aColor;
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}