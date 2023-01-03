#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <numeric>

extern int randint(int start, int end);
extern float randfloat(float start, float end);
extern float dotProduct(std::vector<float> arr1, std::vector<float> arr2);

template <typename T>
void print(const T& x);

template <typename T>
T randChoice(const std::vector<T>& v);

template <typename T>
void printArr(const std::vector<T>& v);

template <typename T>
void remove_from_vector(std::vector<T>& vec, const T& element);

extern std::string floatToString(double num);
extern std::string calcFPS(sf::Clock& clock);


// src/other_utils/UsefulFunctions.cpp