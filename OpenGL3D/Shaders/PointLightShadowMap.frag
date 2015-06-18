#version 330

in vec3 outWorldPosition;

uniform vec3 lightWorldPos;

out float fragColor;

void main()
{
    //vec3 toVertexVector = outWorldPosition - lightWorldPos;
	//float dist = length(toVertexVector);
	fragColor = distance(outWorldPosition, lightWorldPos);
}