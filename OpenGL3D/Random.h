#pragma once

#include <random>
#include <ctime>

class Random
{
public:
	Random() : Random(time(NULL)) {}
	Random(int seed);
	~Random();

	int nextInt(int n);
	float nextFloat();
	void setSeed(int n);

private:
	std::default_random_engine engine;
};

