#pragma once

#include <vector>

extern unsigned int randint(const unsigned int& start, const unsigned int& end);
extern float randfloat(const float& start, const float& end);
extern float dotProduct(const std::vector<float>& arr1, const std::vector<float>& arr2);

template <typename T>
T randChoice(const std::vector<T>& v);

template <typename T>
void printArr(const std::vector<T>& v);

template <typename T>
void remove_from_vector(std::vector<T>& vec, const T& element);



// src/other_utils/UsefulFunctions.cpp