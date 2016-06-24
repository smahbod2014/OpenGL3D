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

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform DirectionalLight dLight;

void main()
{
	vec4 blendMapColor = texture(blendMap, outTexCoord);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = outTexCoord * 40.0;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;
	
	vec4 textureColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	
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

