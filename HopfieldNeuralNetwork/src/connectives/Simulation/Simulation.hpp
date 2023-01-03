#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <memory>

#include "../../rendering/NeuronRendering.hpp"
#include "../../rendering/Buffer.hpp"
#include "../NeuralNetwork/NeuralNetwork.hpp"
#include "../../screen_utils/button.hpp"
#include "../../memory_utils/MemoryMaker.hpp"
#include "../../memory_utils/MemoryManager.hpp"
#include "../../other_utils/ObjectManager.hpp"



class Simulation {
public:
	const unsigned int p_neuronsPerAxies = 150;
	const unsigned int p_neuronAmount = p_neuronsPerAxies * p_neuronsPerAxies;


private: // variables

	// boolians!!!
	bool m_paused = false;
	bool m_mouseCurrentlyPressed = false;

	// constants :]
	const unsigned int m_neuronShapeSides = 5;
	const unsigned int m_button_char_size = 20;
	const float m_neuronRadius = 2.0f;

	// colors
	const sf::Color m_neuronColorNegative{ 0, 0, 0 };
	const sf::Color m_neuronColorPositive{ 205, 205, 255 };
	const sf::Color m_buttonColorOff{ 30, 30, 30 };
	const sf::Color m_buttonColorOn{ 20, 100, 122 };

	// borders
	Rect m_neuralNetworkBorder{};
	Rect m_memoryMakerBorder{};
	Rect m_memoryManagerBorder{};

	// containers
	ObjectManager< ObjectManager<Button> > m_buttons;
	std::vector<Neuron> m_neurons{};
	

	// custom classes
	ArrayOfCircles m_buffer{};
	NeuralNetwork m_neuralNetwork{};
	Renderer m_renderer{};
	MemoryMaker m_memoryMaker{};
	MemoryManager m_memoryManager{};

	// other stuff
	sf::Font m_font{};
	sf::Vector2i m_mousePosition{};


private: // functions private
	void initiliseCircles();
	void initiliseNeurons();
	void initiliseNeuralNetwork();
	void initiliseRenderer();
	void initiliseMemoryManager();
	void initiliseMemoryMaker();

	// button functions
	void addMemory();
	void loadMemory();

	// wrapper function
	bool checkMemoryClick(sf::Vector2i mousePosition, bool clicked, bool drawMode, bool drawSize);

	void update_weights(int num_neurons, double* outputs, double** weights, double learning_rate);
	std::vector<double> storkey_learning_rule(std::vector<double> weights, std::vector<double> inputs, double error, double learning_rate);

	// events
	bool handleCloseEvent(sf::RenderWindow& window, const sf::Event& event);
	bool handleKeyPressEvent(sf::RenderWindow& window, const sf::Event& event);
	bool handleMouseButtonPressEvent(const sf::Event& event, sf::Vector2i mousePosition);
	bool handleMouseButtonReleaseEvent(const sf::Event& event);
	bool buttonPressedInsideVector(std::string buttonArrayName, sf::Vector2i mousePosition);
	void manageOptionButtons(std::string name);


public: // functions public
	Simulation();

	void initNeuronInfo(Rect neuralNetworkBorder, Rect memoryMakerBorder, Rect memoryManagerBorder);

	void memoryMakerClickCheck();

	// event checks
	void eventCheck(sf::Event& event, sf::RenderWindow& window, sf::Vector2i mousePosition);

	// button stuff
	void addButton(std::string text, Rect rect, std::string addToList, bool isToggleButton);
	Button* getButton(std::string buttonList, std::string buttonName);

	void updateAndRender(sf::RenderWindow& window);
};