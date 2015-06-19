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

const int POINT_LIGHT = 0;
const int SPOT_LIGHT = 1;

const int MAX_SPOT_LIGHTS = 2;
const int MAX_POINT_LIGHTS = 3;
const vec4 dummy = vec4(0.0, 0.0, 0.0, 0.0);

in vec3 outWorldPosition;
in vec2 outTexCoord;
in vec3 outNormal;
in vec4 spotLightSpacePos[MAX_SPOT_LIGHTS];
in vec4 directionalLightSpacePos;

out vec4 finalColor;

uniform sampler2D sampler;
uniform sampler2D gShadowMap[MAX_SPOT_LIGHTS];
uniform sampler2D directionalShadowMap;
uniform samplerCube gShadowCubeMap[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLight;


float calcShadowFactor(vec4 lightSpacePos, int i);
float calcShadowFactorDirectional(vec4 lightSpacePos, float bias);
float calcShadowFactorCube(vec3 lightDirection, int i);
vec4 calcPointLight(PointLight pl, vec4 lightSpacePos, int i, int baseLightType);
vec4 calcSpotLight(SpotLight sl, vec4 lightSpacePos, int i);
vec4 calcDirectionalLight(DirectionalLight dl, vec4 direcLightSpacePos);

void main()
{
	vec4 textureColor = texture(sampler, outTexCoord);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 totalLighting = vec4(0.0, 0.0, 0.0, 0.0);
	
	if (directionalLight.base.isActive == 1)
		totalLighting += calcDirectionalLight(directionalLight, directionalLightSpacePos);
	
	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
		if (spotLights[i].base.base.isActive == 1)
			totalLighting += calcSpotLight(spotLights[i], spotLightSpacePos[i], i);
	
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		if (pointLights[i].base.isActive == 1)
			totalLighting += calcPointLight(pointLights[i], dummy, i, POINT_LIGHT);
	
	finalColor = textureColor * clamp(totalLighting + ambient, 0.0, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight dl, vec4 direcLightSpacePos)
{
	vec3 toLightVector = -dl.direction;
	float dotFactor = dot(outNormal, toLightVector);
	float coeff = max(0.0, dotFactor);
	float bias = 0.005 * tan(acos(dotFactor));
	bias = clamp(bias, 0.003, 0.01);
	float shadowFactor = calcShadowFactorDirectional(direcLightSpacePos, bias);
	return vec4(dl.base.color * coeff * shadowFactor, 1.0);
}

vec4 calcPointLight(PointLight pl, vec4 lightSpacePos, int i, int baseLightType)
{
	vec3 toLightVector = pl.position - outWorldPosition;
	float d = length(toLightVector);
	float attenuation = 1.0 / (pl.attenuation.constantAtten
		+ pl.attenuation.linearAtten * d
		+ pl.attenuation.quadraticAtten * d * d);
	float coeff = max(0.0, dot(outNormal, normalize(toLightVector)));
	
	float shadowFactor = 1.0;
	if (baseLightType == SPOT_LIGHT)
		shadowFactor = calcShadowFactor(lightSpacePos, i);
	else if (baseLightType == POINT_LIGHT)
		shadowFactor = calcShadowFactorCube(-toLightVector, i);
		
	return vec4(pl.base.color * attenuation * coeff * shadowFactor, 1.0);
}

vec4 calcSpotLight(SpotLight sl, vec4 lightSpacePos, int i)
{
	vec3 toLightVector = sl.base.position - outWorldPosition;
	float spotFactor = dot(normalize(-toLightVector), sl.direction);
	
	if (spotFactor > sl.cutoff)
	{
		vec4 pointLightComponent = calcPointLight(sl.base, lightSpacePos, i, SPOT_LIGHT);
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
	if (depth < z - 0.0001)
		return 0.5;
	else
		return 1.0;
}

float calcShadowFactorDirectional(vec4 lightSpacePos, float bias)
{
	vec3 projectedCoords = lightSpacePos.xyz / lightSpacePos.w;
	vec2 uvCoords;
	uvCoords.x = 0.5 * projectedCoords.x + 0.5;
	uvCoords.y = 0.5 * projectedCoords.y + 0.5;
	float z = 0.5 * projectedCoords.z + 0.5;
	float depth = texture(directionalShadowMap, uvCoords).x;
	if (depth < z - bias)
		return 0.5;
	else
		return 1.0;
}

float calcShadowFactorCube(vec3 toFragVector, int i)
{
	float sampledDistance = texture(gShadowCubeMap[i], toFragVector).r;
	float dist = length(toFragVector);
	if (dist < sampledDistance + 0.0001)
		return 1.0;
	else
		return 0.5;
}