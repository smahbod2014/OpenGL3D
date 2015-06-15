#version 400 core

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

in vec2 passTexCoord;
in vec3 outNormal;
in vec3 toLightVector;
in vec4 lightSpacePosition;

uniform sampler2D sampler;
uniform sampler2D gShadowMap;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform int lightType;
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;
uniform float lightCutoff;

out vec4 finalColor;

float calcShadowFactor(vec4 lightSpacePos);

void main()
{
	finalColor = texture(sampler, passTexCoord);
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
	
	if (lightType == DIRECTIONAL_LIGHT)
	{
		//directional light
		float coeff = max(0.2, dot(outNormal, normalize(-lightPosition)));
		finalColor *= coeff * vec4(lightColor, 1.0);
	}
	else if (lightType == POINT_LIGHT)
	{
		//point light
		float d = length(toLightVector);
		float attenuation = 1.0 / (lightConstant + lightLinear * d + lightQuadratic * d * d);
		float coeff = max(0.2, dot(outNormal, normalize(toLightVector)));
		finalColor *= coeff * attenuation * vec4(lightColor, 1.0);
	}
	else if (lightType == SPOT_LIGHT)
	{                             
		float spotFactor = dot(normalize(-toLightVector), lightDirection);                                  
		float shadowFactor = calcShadowFactor(lightSpacePosition);
		//shadowFactor = 1.0;
		
		if (spotFactor > lightCutoff) {                                                            
			float d = length(toLightVector);
			float attenuation = 1.0 / (lightConstant + lightLinear * d + lightQuadratic * d * d);
			float coeff = max(0.2, dot(outNormal, normalize(toLightVector)));
			float fading = (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - lightCutoff));
			finalColor *= coeff * attenuation * fading * shadowFactor * vec4(lightColor, 1.0);
			//finalColor += 0.1;
		}                                                                                       
		else {                                                                                  
			finalColor *= 0;                                                               
		}
	}
}

float calcShadowFactor(vec4 lightSpacePos)
{
	vec3 projectedCoords = lightSpacePos.xyz / lightSpacePos.w;
	vec2 uvCoords;
	uvCoords.x = 0.5 * projectedCoords.x + 0.5;
	uvCoords.y = 0.5 * projectedCoords.y + 0.5;
	float z = 0.5 * projectedCoords.z + 0.5;
	float depth = texture(gShadowMap, uvCoords).x;
	if (depth < z - 0.00001)
		return 0.5;
	else
		return 1.0;
}