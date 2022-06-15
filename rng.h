// Rng.h
// Tiny header file to help make uniform random numbers
// Credit goes to https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
// I used it for help.
#ifndef _RNG_H_
#define _RNG_H_
#include <random>

template<typename T>
T random(T min, T max) {
	if (max < min) {
		T temp = min;
		min = max;
		max = temp;
	}
	std::random_device rng;
	// Mersenne twister.
	std::mt19937 generator(rng());
	std::uniform_int_distribution<T> distr(min, max);
	return distr(generator);
}
#endif
