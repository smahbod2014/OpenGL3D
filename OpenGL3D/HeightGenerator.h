#pragma once

#include <map>
#include "Random.h"

#define HEIGHT_SMOOTHING 4.0f
#define OCTAVES 3
#define ROUGHNESS 0.2f

class HeightGenerator
{
public:
	HeightGenerator(float amplitude, int width);
	~HeightGenerator();

	float generateHeight(int x, int z);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);
	float getInterpolatedNoise(float x, float z);
	float interpolate(float a, float b, float blend);

private:
	std::map<int, float> noiseCache;
	float amplitude;
	int width;
	Random* random;
};

