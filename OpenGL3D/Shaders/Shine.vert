#version 400 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform mat4 M = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 P = mat4(1.0);
uniform mat4 depthBiasMatrix = mat4(1.0);

uniform vec3 lightPosition;

out vec2 passTexCoord;
out vec3 outNormal;
out vec3 toLightVector;

out vec4 shadowCoord;

void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = P * V * worldPosition;
	passTexCoord = texCoord;
	
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	toLightVector = lightPosition - worldPosition.xyz;
	
	//---
	shadowCoord = depthBiasMatrix * vec4(position, 1.0);
}