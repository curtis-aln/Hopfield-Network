#pragma once

#include <SFML/Graphics.hpp>

struct CalculatorNeuron {
	float state{};
	int id{};
	sf::Vector2f myStates{};


	CalculatorNeuron(int identity = -1, float N_state = -1, sf::Vector2f possibleStates = { -1.0f, 1.0f }) {
		id = identity;
		state = N_state;
		myStates = possibleStates;
	}


	void flipState() {
		state *= -1;
	}


	bool stateAsBool() {
		if (state > 0) return true;
		return false;
	}
};