#version 400 core

in vec3 textureCoords;

out vec4 fragColor;

uniform samplerCube cubeMap;

void main()
{
	fragColor = texture(cubeMap, textureCoords);
}