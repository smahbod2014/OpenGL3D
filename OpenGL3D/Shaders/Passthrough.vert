#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 VP;
uniform mat4 M;

out vec2 outTexCoord;

void main()
{
    gl_Position = VP * M * vec4(Position, 1.0);
	outTexCoord = TexCoord;
}