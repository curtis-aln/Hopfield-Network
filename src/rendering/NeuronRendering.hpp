#pragma once

#include <iostream>
#include <array>
#include <unordered_set>

#include <SFML/Graphics.hpp>
#include "../neuron_utils/Neuron.hpp"
#include "../rendering/Buffer.hpp"
#include "../connectives/NeuralNetwork/NeuralNetwork.hpp"


namespace std {
	template <> struct hash<std::array<sf::Vector2f, 2>> {
		inline size_t operator()(const std::array<sf::Vector2f, 2>& v) const noexcept
        {
            constexpr std::hash<float> int_hasher;
            const size_t h1 = int_hasher(v[0].x) ^ int_hasher(v[0].y);
            const size_t h2 = int_hasher(v[1].x) ^ int_hasher(v[1].y);
			return h1 + h2;
		}
	};
}



class Renderer {
private:
	int m_neuronsPerAxies{};

	Rect m_border{};

	// the m_neuralNetwork is a pointer to the Neural Network class inside of NeuralNetwork.hpp
	// the wrappedNeurons vector is a vector full of the wrapped Neuron class
	NeuralNetwork* m_neuralNetwork{};

	// this vertex array is used for drawing the synapses of the neurons for optimisation
	sf::VertexArray m_synapses{};


public:
	Renderer() = default;

    void initiliseRenderer(NeuralNetwork& neuralNetwork, int neuronsPerAxies,
                           float neuronRadius, const Rect& border, sf::VertexArray& circles,
                           std::vector<Neuron>& neurons) {

		// initilising data
		m_neuralNetwork = &neuralNetwork;
		m_neuronsPerAxies = neuronsPerAxies;
		m_border = border;

		// creating the neurons array
		puts("initilising neuron wrappers . . .");
		initNeurons(border.x + neuronRadius, border.y + neuronRadius, neuronRadius, circles, neurons);
		puts("neuron wrappers initilised.");

	}


	void drawSynapses(sf::RenderWindow& window) const
    {
		window.draw(m_synapses);
	}

	void drawBoundingBox(sf::RenderWindow& window) const
    {
		sf::RectangleShape boundingBox;
		boundingBox.setFillColor(sf::Color(0, 0, 0, 0));
		boundingBox.setOutlineColor(sf::Color(0, 0, 200));
		boundingBox.setPosition(sf::Vector2f(m_border.x, m_border.y));
		boundingBox.setSize(sf::Vector2f(m_border.w, m_border.h));
		boundingBox.setOutlineThickness(2);

		window.draw(boundingBox);
	}


private:
	float calcNeuronSize(float neuronsPerAxies, float neuronSpacing) const
    {
		return (m_border.w / neuronsPerAxies) - neuronSpacing;
	}

	void initNeurons(float startX, float startY, float neuronSize, sf::VertexArray& circles, std::vector<Neuron>& neurons) const
    {
        const float spacing = m_border.w / m_neuronsPerAxies;
		for (int i = 0; i < m_neuronsPerAxies; i++) {
			for (int j = 0; j < m_neuronsPerAxies; j++) {
				float x = startX + (i * spacing);
				float y = startY + (j * spacing);

                const int current = i + j * m_neuronsPerAxies;
				neurons[current].setPosition(circles, { x, y });
			}
		}

		puts("[initNeurons()] -> neuronWrapper objects initilised");

	}
};