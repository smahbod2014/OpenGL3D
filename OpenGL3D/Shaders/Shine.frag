#version 400 core

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;

in vec2 passTexCoord;
in vec3 outNormal;
in vec3 toLightVector;

in vec4 shadowCoord;

uniform sampler2D sampler;
uniform sampler2D shadowMap;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform int lightType;
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;

out vec4 color;

void main()
{
#if 0
	float visibility = texture( shadowMap, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w) );
#else
	float visibility = 1;
	if (texture(shadowMap, shadowCoord.xy).z < shadowCoord.z)
		visibility = 0.5;
#endif
		
	color = texture(sampler, passTexCoord);
	
	if (lightType == DIRECTIONAL_LIGHT)
	{
		//directional light
		float coeff = max(0.2, dot(outNormal, normalize(-lightPosition)));
		color *= visibility * coeff * vec4(lightColor, 1.0);
		//color = vec4(1, 1, visibility, 1.0);
	}
	else if (lightType == POINT_LIGHT)
	{
		//point light
		float d = length(toLightVector);
		float attenuation = 1.0 / (lightConstant + lightLinear * d + lightQuadratic * d * d);
		float coeff = max(0.2, dot(outNormal, normalize(toLightVector)));
		color *= visibility * coeff * attenuation * vec4(lightColor, 1.0);
	}
}