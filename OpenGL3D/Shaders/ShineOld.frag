
uniform float t;

out vec4 color;

void main()
{
	float r = abs(sin(t / 2.0));
	float g = abs(cos(t / 3.0 + 1.0));
	float b = 0.75;
	//gl_FragColor = vec4(r, g, b, 1.0);
	color = vec4(r, g, b, 1.0);
}