#version 400 core

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

uniform mat4 M = mat4(1.0);
uniform mat4 V;
uniform mat4 P;
uniform vec4 plane;
uniform float hasNormalMap;

out vec2 outTexCoord;
out vec3 outNormal;
out vec3 outWorldPosition;
out mat3 toTangentSpace;

void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	gl_Position = P * V * worldPosition;
	outTexCoord = texCoord;
	//todo: normal will not work for non-uniform scaling
	outNormal = normalize((M * vec4(normal, 0.0)).xyz);
	outWorldPosition = worldPosition.xyz;
	
	gl_ClipDistance[0] = dot(worldPosition, plane);

	toTangentSpace = mat3(1.0);
	if (hasNormalMap > 0.5) {
		vec3 norm = outNormal;
		vec3 tang = normalize((M * vec4(tangent, 0.0)).xyz);
		vec3 bitang = normalize(cross(norm, tang));
		toTangentSpace = transpose(mat3(tang, bitang, norm));
	}
}