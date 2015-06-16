#version 400 core

//lighting data structures

struct BaseLight
{
	int isActive;
	vec3 color;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct Attenuation
{
	float constantAtten;
	float linearAtten;
	float quadraticAtten;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	Attenuation attenuation;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutoff;
};

//-----------

const int NUM_SPOTLIGHTS = 5;

in vec3 outWorldPosition;
in vec2 passTexCoord;
in vec3 outNormal;
in vec4 lightSpacePosition[NUM_SPOTLIGHTS];

out vec4 finalColor;


uniform sampler2D sampler;
uniform sampler2D gShadowMap[NUM_SPOTLIGHTS];
uniform SpotLight spotLights[NUM_SPOTLIGHTS];


float calcShadowFactor(vec4 lightSpacePos, int i);
vec4 calcPointLight(PointLight pl, vec4 lightSpacePos, int i);
vec4 calcSpotLight(SpotLight sl, vec4 lightSpacePos, int i);

void main()
{
	vec4 textureColor = texture(sampler, passTexCoord);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 totalLighting = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < NUM_SPOTLIGHTS; i++)
		if (spotLights[i].base.base.isActive == 1)
			totalLighting += calcSpotLight(spotLights[i], lightSpacePosition[i], i);
	
	finalColor = textureColor * clamp(totalLighting + ambient, 0.0, 1.0);
}

vec4 calcPointLight(PointLight pl, vec4 lightSpacePos, int i)
{
	vec3 toLightVector = pl.position - outWorldPosition;
	float d = length(toLightVector);
	float attenuation = 1.0 / (pl.attenuation.constantAtten
		+ pl.attenuation.linearAtten * d
		+ pl.attenuation.quadraticAtten * d * d);
	float coeff = max(0.0, dot(outNormal, normalize(toLightVector)));
	float shadowFactor = calcShadowFactor(lightSpacePos, i);
	return vec4(pl.base.color * attenuation * coeff * shadowFactor, 1.0);
}

vec4 calcSpotLight(SpotLight sl, vec4 lightSpacePos, int i)
{
	vec3 toLightVector = sl.base.position - outWorldPosition;
	float spotFactor = dot(normalize(-toLightVector), sl.direction);
	
	if (spotFactor > sl.cutoff)
	{
		vec4 pointLightComponent = calcPointLight(sl.base, lightSpacePos, i);
		float fading = (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - sl.cutoff));
		return pointLightComponent * fading;
	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
}

float calcShadowFactor(vec4 lightSpacePos, int i)
{
	vec3 projectedCoords = lightSpacePos.xyz / lightSpacePos.w;
	vec2 uvCoords;
	uvCoords.x = 0.5 * projectedCoords.x + 0.5;
	uvCoords.y = 0.5 * projectedCoords.y + 0.5;
	float z = 0.5 * projectedCoords.z + 0.5;
	float depth = texture(gShadowMap[i], uvCoords).x;
	if (depth < z - 0.00001)
		return 0.5;
	else
		return 1.0;
}