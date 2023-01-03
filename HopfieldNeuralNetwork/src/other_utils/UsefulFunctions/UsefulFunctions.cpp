
#include "UsefulFunctions.hpp"

// random
int randint(int start, int end) {
	return (int)rand() % ((end + 1) - start) + start;
}


float randfloat(float start, float end) {
	return (float(rand()) / float((RAND_MAX)) * (end - start)) + start;
}


template <typename T>
T randChoice(const std::vector<T>& v) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, v.size() - 1);

	return v[dis(gen)];
}


// dot product
float dotProduct(std::vector<float> arr1, std::vector<float> arr2) {
	// gets the dot product (float) of two arrays
	return std::transform_reduce(arr1.begin(), arr1.end(), arr2.begin(), 0);
}

// printing
template <typename T>
void print(const T& x) {
	std::cout << x << std::endl;
}


template <typename T>
void printArr(const std::vector<T>& vec) {
	std::cout << "[";
	for (size_t i = 0; i < vec.size(); ++i) {
		const T& element = vec[i];
		print(element);
		if (i < vec.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << "]" << std::endl;
}

// vector
template <typename T>
void remove_from_vector(std::vector<T>& vec, const T& element) {
	auto new_end = std::stable_partition(vec.begin(), vec.end(), [&](const T& y) { return y != element; });
	vec.resize(std::distance(vec.begin(), new_end));
}

// other
std::string floatToString(double num) {
	std::ostringstream oss;
	oss << "" << num << "";
	return oss.str();
}

std::string calcFPS(sf::Clock& clock) {
	// FPS management
	sf::Int64 timePerFrame = clock.restart().asMicroseconds();

	std::ostringstream oss;
	oss << timePerFrame;
	return oss.str();
}