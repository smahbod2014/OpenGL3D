#version 400 core

const int NUM_SPOT_LIGHTS = 2;

/*in vec3 position;
in vec2 texCoord;
in vec3 normal;*/
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 M = mat4(1.0);
uniform mat4 VP;
uniform mat4 spotLightVP[NUM_SPOT_LIGHTS];


out vec2 outTexCoord;
out vec3 outNormal;
out vec3 outWorldPosition;
out vec4 spotLightSpacePos[NUM_SPOT_LIGHTS];


void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = VP * worldPosition;
	outTexCoord = texCoord;
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	outWorldPosition = worldPosition.xyz;
	
	//Calculate the vertex's position as seen from the spot light source
	for (int i = 0; i < NUM_SPOT_LIGHTS; i++)
		spotLightSpacePos[i] = spotLightVP[i] * worldPosition;
}