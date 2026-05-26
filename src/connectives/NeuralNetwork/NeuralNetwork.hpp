#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../neuron_utils/Neuron.hpp"
#include "../../rendering/Buffer.hpp"

#include <algorithm>
#include <random>
#include <vector>
#include "../../other_utils/UsefulFunctions/UsefulFunctions.hpp"


struct Weights {
	std::vector< std::vector < float > > weightsAsVector;
};


class NeuralNetwork {
public:
	sf::Vector2i m_neuronStateRange{};

	std::vector<Neuron>* m_neurons{};
	std::vector<std::vector<float>> m_weights{};
	std::vector<int> m_indexes{};

	int m_neuronsIteratedCount = 0;
	int m_tickspeed = 1;

	float m_neuronPositionDetiriationStrength = 0.05f;
	float m_neuronNoiseStrength = 0.1f;
	float m_synapseDetiriationStrength = 1.2f;
	float m_synapseDetiriationAmount = 2555;


private:
	void getWeightsAndStates(std::vector<float>& foundWeights, std::vector<float>& foundStates, Neuron& neuronInQuestion) const;

	Weights convertMemoryToWeights(const std::vector<float>& memory);

	float queryWeight(int neuron1_id, int neuron2_id) const;
	float collapseState(float x) const;

	int neuronToUpdate();


public:
	std::vector<Weights> turnMemoriesToWeights(std::vector<std::vector<float>>& memories);
	Weights mergeWeights(const std::vector< Weights >& all_weights);
	void hardSetWeights(const std::vector<std::vector<float>>& weights);

	void initiliseNetwork(std::vector<Neuron>& N_neurons, std::vector< std::vector< float> > allMemories);
	void initMemory(std::vector< std::vector<float>> memories);
	void randomiseMemoryStates(sf::VertexArray& circles) const;
	void flipTickSpeed();
	void updateStates(sf::VertexArray& circles);


	static int getRandomState(sf::Vector2f possibleStates);
	static void appendRandomMemory(std::vector<std::vector<float>>& allMemories, const unsigned int amount);
	static std::vector<Neuron> generateNeurons(int amount, ArrayOfCircles& buffer,
		sf::Color colorOff, sf::Color colorOn, sf::Vector2f possibleStates);
};