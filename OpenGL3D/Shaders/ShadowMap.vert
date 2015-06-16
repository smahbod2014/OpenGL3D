#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 lightMVP;

out vec2 TexCoordOut;

void main()
{
    gl_Position = lightMVP * vec4(Position, 1.0);
    TexCoordOut = TexCoord;
}