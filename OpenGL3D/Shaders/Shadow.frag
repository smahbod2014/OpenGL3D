#version 400 core

out vec4 out_colour;

uniform sampler2D modelTexture;//will use this next week

void main()
{
	out_colour = vec4(1.0, 0, 0, 1);
}