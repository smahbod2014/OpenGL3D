#version 400 core

const int MAX_SPOT_LIGHTS = 2;

/*in vec3 position;
in vec2 texCoord;
in vec3 normal;*/
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 M = mat4(1.0);
uniform mat4 VP;
uniform mat4 spotLightVP[MAX_SPOT_LIGHTS];
uniform mat4 directionalLightVP;


out vec2 outTexCoord;
out vec3 outNormal;
out vec3 outWorldPosition;
out vec4 spotLightSpacePos[MAX_SPOT_LIGHTS];
out vec4 directionalLightSpacePos;

void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = VP * worldPosition;
	outTexCoord = texCoord;
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	outWorldPosition = worldPosition.xyz;
	
	//Calculate the vertex's position as seen from the spot light source
	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
		spotLightSpacePos[i] = spotLightVP[i] * worldPosition;
		
	//same thing but for the directional light, if any
	directionalLightSpacePos = directionalLightVP * worldPosition;
}