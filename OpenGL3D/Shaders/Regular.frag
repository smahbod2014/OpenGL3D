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
in mat3 toTangentSpace;

out vec4 finalColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform DirectionalLight dLight;
uniform float hasNormalMap;
uniform float hasSpecularMap;
uniform vec3 cameraPosition;
uniform float specularDamper;
uniform float specularReflectivity;

void main()
{
	//sample the diffuse texture map
	vec4 textureColor = texture(diffuseMap, outTexCoord);
	if (textureColor.a < 0.5)
		discard;
	
	//sample the normal map, if any
	vec3 normal = outNormal;
	if (hasNormalMap > 0.5) {
		normal = normalize(2.0 * texture(normalMap, outTexCoord).rgb - 1.0);
	}
	
	vec3 toCameraVector = normalize(toTangentSpace * (cameraPosition - outWorldPosition));
	vec3 totalDiffuse = vec3(0.0, 0.0, 0.0);
	vec3 totalSpecular = vec3(0.0, 0.0, 0.0);
	if (dLight.base.isActive > 0.5) {
		vec3 toLightVector = toTangentSpace * -dLight.direction;
		float coeff = max(0.2, dot(toLightVector, normal));
		vec3 dLightContribution = coeff * dLight.base.color;
		vec3 reflected = reflect(-toLightVector, normal);
		float specBrightness = max(0.0, dot(reflected, toCameraVector));
		float specDamping = pow(specBrightness, specularDamper);
		vec3 finalSpecular = specDamping * dLight.base.color * specularReflectivity;
		totalDiffuse += dLightContribution;
		totalSpecular += finalSpecular;
	}
	
	//sample the specular map, if any
	if (hasSpecularMap > 0.5) {
		vec4 mapInfo = texture(specularMap, outTexCoord);
		totalSpecular *= mapInfo.r;
		if (mapInfo.g > 0.0) {
			totalDiffuse = vec3(1.0, 1.0, 1.0);
		}
	}
	
	finalColor = textureColor * clamp(vec4(totalDiffuse, 1.0), 0.0, 1.0) + vec4(totalSpecular, 1.0);
	//finalColor = vec4(totalLighting, 1.0);
}

