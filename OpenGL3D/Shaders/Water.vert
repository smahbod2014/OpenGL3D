#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 M = mat4(1.0);
uniform mat4 V;
uniform mat4 P;

out vec2 textureCoords;

void main()
{
	gl_Position = P * V * M * vec4(position, 1.0);
	textureCoords = vec2(position.x / 2.0 + 0.5, position.z / 2.0 + 0.5);
}