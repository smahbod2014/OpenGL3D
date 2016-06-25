#include "HeightGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

HeightGenerator::HeightGenerator(float amplitude, int width)
{
	this->amplitude = amplitude;
	this->width = width;
	random = new Random(1);
}


HeightGenerator::~HeightGenerator()
{
}

float HeightGenerator::generateHeight(int x, int z)
{
	//return getInterpolatedNoise(x / HEIGHT_SMOOTHING, z / HEIGHT_SMOOTHING) * amplitude;

	/*float total = getInterpolatedNoise(x / HEIGHT_SMOOTHING, z / HEIGHT_SMOOTHING) * amplitude;
	total += getInterpolatedNoise(x / (HEIGHT_SMOOTHING / 2.0f), z / (HEIGHT_SMOOTHING / 2.0f)) * amplitude / 3.0f;
	total += getInterpolatedNoise(x / (HEIGHT_SMOOTHING / 4.0f), z / (HEIGHT_SMOOTHING / 4.0f)) * amplitude / 9.0f;
	return total;*/

	float total = 0;
	float d = (float)powf(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float freq = (float)(powf(2, i) / d);
		float amp = (float)(powf(ROUGHNESS, i) * amplitude);
		total += getInterpolatedNoise(x * freq, z * freq) * amp;
	}
	return total;
}

float HeightGenerator::getNoise(int x, int z)
{
	int i = z * width + x;
	if (noiseCache.find(i) == noiseCache.end()) {
		noiseCache[i] = random->nextFloat() * 2.0f - 1.0f;
	}
	return noiseCache[i];
}

float HeightGenerator::getSmoothNoise(int x, int z)
{
	float corners = (getNoise(x - 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z - 1) + getNoise(x + 1, z + 1)) / 16.0f;
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;
	float center = getNoise(x, z) / 4.0f;
	return corners + sides + center;
}

float HeightGenerator::getInterpolatedNoise(float x, float z)
{
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;
	
	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightGenerator::interpolate(float a, float b, float blend)
{
	double theta = M_PI * blend;
	//float f = cosf((float)theta) * 0.5f + 0.5f;
	float f = (float)(1.0f - cosf((float)theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}