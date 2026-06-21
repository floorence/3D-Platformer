#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;

layout (location = 3) in vec2 aTex;

// Outputs the color for the Fragment Shader
out vec3 color;
out vec2 texCoord;

uniform mat4 projection;

void main() {
	color = aColor;
	texCoord = aTex;

	gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}