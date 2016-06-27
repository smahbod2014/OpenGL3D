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
in vec4 shadowCoords;

out vec4 finalColor;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;
uniform sampler2D shadowMap;
uniform DirectionalLight dLight;
uniform int shadowMapSize;
uniform int pcfCount;

void main()
{
	float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
	float shadowMapSize = float(shadowMapSize);
	float texelSize = 1.0 / shadowMapSize;
	float pcfTotal = 0.0;
	for (int x = -pcfCount; x <= pcfCount; x++) {
		for (int y = -pcfCount; y <= pcfCount; y++) {
			vec2 offset = vec2(x, y) * texelSize;
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + offset).r;
			if (shadowCoords.z > objectNearestLight) {
				pcfTotal += 1.0;
			}
		}
	}
	
	float lightFactor = 1.0 - (pcfTotal / totalTexels * shadowCoords.w);
	
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
		float coeff = max(0.3, dot(-dLight.direction, outNormal) * lightFactor);
		vec3 dLightContribution = coeff * dLight.base.color;
		totalLighting += dLightContribution;
	}
	
	finalColor = textureColor * clamp(vec4(totalLighting, 1.0), 0.0, 1.0);
}

