#version 330

in vec2 outTexCoord;

uniform sampler2D sampler;

out vec4 fragColor;

void main()
{
	fragColor = texture(sampler, outTexCoord);
}