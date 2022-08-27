// Rng.h
// Tiny header file to help make uniform random numbers
// Credit goes to https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
// I used it for help.
#ifndef _RNG_H_
#define _RNG_H_

#include <random>
#include <utility>

int randint(int min, int max) {
	if (max < min) {
		int temp(std::move(min));
		min = std::move(max);
		max = std::move(temp);
	}
	std::random_device rng;
	// Mersenne twister.
	std::mt19937 generator(rng());
	std::uniform_int_distribution<int> distr(min, max);
	return distr(generator);
}

float uniform(float min, float max) {
    if (max < min) {
        float temp(std::move(min));
        min = std::move(max);
        max = std::move(temp);
    }
    std::random_device rng;
    std::mt19937 generator(rng());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(generator);
}

#endif
