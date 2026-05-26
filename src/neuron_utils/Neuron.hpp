#pragma once


#include <iostream>
#include <SFML/Graphics.hpp>

#include "../neuron_utils/NeuronCalculator.hpp"
#include "../rendering/Buffer.hpp"
#include "../other_utils/Rect.hpp"

/*
The neuron class puts together the circle from Buffer.hpp and the calculator
from NeuronCalculator.hpp to create a simple to use, easy to manipulate Neuron.

`Calculator Neuron`
The purpose of the Calculator is to determine the state of the neuron every frame

`Buffer circle`
Controls the graphics, shape and color of the Neuron

`ArrayOfCircles`
this class is incharge of managing the Circles mentioned above, we can create a refrence to those circles

---------------------------------------------
We first create the circles inside of the main() function in main.py
then we use a static function from NeuralNetwork to generate an std::vector<Neuron>
after that we pass it into Renderer to set the positions of the neurons and manage them
*/



class Neuron {
private:
	CalculatorNeuron m_calculatorNeuron{};
	Circle* m_renderNeuron{};

	sf::Color m_colorOff{};
	sf::Color m_colorOn{};


public:
	// Constuctor
	Neuron(sf::VertexArray& circles, CalculatorNeuron calculatorNeuron, Circle& renderNeuron, sf::Color colorOff, sf::Color colorOn) {
		m_calculatorNeuron = calculatorNeuron;
		m_renderNeuron = &renderNeuron;

		m_colorOff = colorOff;
		m_colorOn = colorOn;

		// setting the color
		m_renderNeuron->setColor(circles, stateToColor());
	}


	sf::Color stateToColor() {
		std::uint8_t col = (std::uint8_t)(127.5 * (double)(m_calculatorNeuron.state+1));
		return sf::Color( col, col, col );
	}


	// wrapper functions
	void flipState(sf::VertexArray& circles) {
		m_calculatorNeuron.flipState();
		m_renderNeuron->setColor(circles, stateToColor());
	}


	void setPosition(sf::VertexArray& circles, sf::Vector2f position) {
		m_renderNeuron->setPosition(circles, position.x, position.y);
	}

	sf::Vector2f getPosition(sf::VertexArray& circles) {
		return m_renderNeuron->getCenter(circles);
	}


	void move(sf::VertexArray& circles, float deltaX, float deltaY) {
		m_renderNeuron->move(circles, deltaX, deltaY);
	}


	bool stateAsBool() {
		return m_calculatorNeuron.stateAsBool();
	}


	float getState() {
		return m_calculatorNeuron.state;
	}


	void setState(sf::VertexArray& circles, float s) {
		m_calculatorNeuron.state = s;
		m_renderNeuron->setColor(circles, stateToColor());
	}


	int id(int i = -1) {
		if (i != -1)
			m_calculatorNeuron.id = i;
		return m_calculatorNeuron.id;
	}
};