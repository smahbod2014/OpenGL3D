#version 400 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;

uniform mat4 M = mat4(1.0);
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPosition;

const float tiling = 4.0;

void main()
{
	vec4 worldPosition = M * vec4(position, 1.0);
	clipSpace = P * V * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
	toCameraVector = cameraPosition - worldPosition.xyz;
}