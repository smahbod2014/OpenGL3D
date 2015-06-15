#version 400 core

/*in vec3 position;
in vec2 texCoord;
in vec3 normal;*/
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 M = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 P = mat4(1.0);
uniform mat4 lightWVP;

uniform vec3 lightPosition;

out vec2 passTexCoord;
out vec3 outNormal;
out vec3 toLightVector;
out vec4 lightSpacePosition;


void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = P * V * worldPosition;
	passTexCoord = texCoord;
	
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	toLightVector = lightPosition - worldPosition.xyz;
	
	//---
	lightSpacePosition = lightWVP * vec4(position, 1.0);
}