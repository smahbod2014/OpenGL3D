#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 M = mat4(1.0);
uniform mat4 V;
uniform mat4 P;
uniform vec4 plane;
uniform mat4 toShadowMapSpace;
uniform float shadowDistance;
uniform float shadowFadeInDistance;

out vec2 outTexCoord;
out vec3 outNormal;
out vec3 outWorldPosition;
out vec4 shadowCoords;

void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = P * V * worldPosition;
	outTexCoord = texCoord;
	//todo: normal will not work for non-uniform scaling
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	outWorldPosition = worldPosition.xyz;
	
	gl_ClipDistance[0] = dot(worldPosition, plane);
	
	shadowCoords = toShadowMapSpace * worldPosition;
	
	vec4 positionRelativeToCam = V * worldPosition;
	float dist = length(positionRelativeToCam.xyz) - (shadowDistance - shadowFadeInDistance);
	dist = dist / shadowFadeInDistance;
	shadowCoords.w = clamp(1.0 - dist, 0.0, 1.0);
}