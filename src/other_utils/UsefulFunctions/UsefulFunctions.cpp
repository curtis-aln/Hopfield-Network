
#include "UsefulFunctions.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

#include <SFML/Graphics.hpp>


// random
unsigned int randint(const unsigned int& start, const unsigned int& end) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution dist6(start, end); // distribution in range [1, 6]

	return dist6(rng);
}


float randfloat(const float& start, const float& end) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution dist6(start, end); // distribution in range [1, 6]

	return dist6(rng);
}

template <typename T>
T randChoice(const std::vector<T>& v) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, v.size() - 1);

	return v[dis(gen)];
}


// dot product
float dotProduct(const std::vector<float>& arr1, const std::vector<float>& arr2) {
	// gets the dot product (float) of two arrays
	return std::transform_reduce(arr1.begin(), arr1.end(), arr2.begin(), 0);
}

// printing
template <typename T>
void printArr(const std::vector<T>& vec) {
	puts("[");
	for (const auto& i : vec) {
		puts(i);
		if (vec.size() - 1) {
			printf(", ");
		}
	}
	puts("]");
}

// vector
template <typename T>
void remove_from_vector(std::vector<T>& vec, const T& element) {
	auto new_end = std::stable_partition(vec.begin(), vec.end(), [&](const T& y) { return y != element; });
	vec.resize(std::distance(vec.begin(), new_end));
}

