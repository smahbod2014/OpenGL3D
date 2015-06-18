#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 lightVP;
uniform mat4 lightM;

out vec3 outWorldPosition;

void main()
{
	vec4 worldPosition = lightM * vec4(Position, 1.0);
    gl_Position = lightVP * worldPosition;
    outWorldPosition = worldPosition.xyz;
}