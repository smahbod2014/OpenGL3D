#include "Random.h"

Random::Random(int seed)
{
	setSeed(seed);
}

Random::~Random()
{
}

int Random::nextInt(int n)
{
	std::uniform_int_distribution<int> distribution(0, n - 1);
	return distribution(engine);
}

float Random::nextFloat()
{
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(engine);
}

void Random::setSeed(int n)
{
	engine.seed(n);
}
