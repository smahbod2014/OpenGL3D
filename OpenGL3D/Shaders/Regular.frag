#version 400 core

struct BaseLight
{
	float isActive;
	vec3 color;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

in vec3 outWorldPosition;
in vec2 outTexCoord;
in vec3 outNormal;

out vec4 finalColor;

uniform sampler2D sampler;
uniform DirectionalLight dLight;

void main()
{
	vec4 textureColor = texture(sampler, outTexCoord);
	
	if (textureColor.a < 0.5)
		discard;
	
	vec3 totalLighting = vec3(0.0, 0.0, 0.0);
	if (dLight.base.isActive == 1.0) {
		float coeff = max(0.0, dot(-dLight.direction, outNormal));
		vec3 dLightContribution = coeff * dLight.base.color;
		totalLighting += dLightContribution;
	}
	
	totalLighting += vec3(0.2, 0.2, 0.2);
	
	finalColor = textureColor * clamp(vec4(totalLighting, 1.0), 0.0, 1.0);
	//finalColor = vec4(1, 0, 0, 1);
}

