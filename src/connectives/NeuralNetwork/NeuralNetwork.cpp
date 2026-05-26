#include "NeuralNetwork.hpp"


// Optimized function for converting a memory vector to a weights matrix
Weights NeuralNetwork::convertMemoryToWeights(const std::vector<float>& memory) {
	Weights weights;

	// Preallocate memory for the weights matrix to improve performance
	weights.weightsAsVector.resize(memory.size(), std::vector<float>(memory.size(), 0.0f));

	// Iterate over the rows and columns of the weights matrix
	for (size_t i = 0; i < memory.size(); i++) {
		for (size_t j = 0; j < memory.size(); j++) {
			// Calculate the element at the i-th row and j-th column of the weights matrix
			weights.weightsAsVector[i][j] = memory[i] * memory[j];
		}
	}

	for (size_t i = 0; i < weights.weightsAsVector.size() - 1; i++) {
		weights.weightsAsVector[i][i] = 0;
	}

	return weights;
}



Weights NeuralNetwork::mergeWeights(const std::vector< Weights >& all_weights) {
	// Preallocate memory for the merged weights matrix to improve performance
	Weights mergedWeights;
    const size_t axiesSize = all_weights[0].weightsAsVector.size();
	mergedWeights.weightsAsVector.resize(axiesSize, std::vector<float>(axiesSize, 0.0f));

	// Iterate over the rows and columns of the merged weights matrix
	for (int i = 0; i < axiesSize; i++) {
		for (int j = 0; j < axiesSize; j++) {
			// Calculate the element at the i-th row and j-th column of the merged weights matrix
			float weightValue = 0;
			for (size_t k = 0; k < all_weights.size(); k++) {
				weightValue += all_weights[k].weightsAsVector[i][j];
			}
			mergedWeights.weightsAsVector[i][j] = weightValue / all_weights.size();
		}
	}

	return mergedWeights;
}



void NeuralNetwork::getWeightsAndStates(std::vector<float>& foundWeights, std::vector<float>& foundStates, Neuron& neuronInQuestion) const
{
	// Preallocate memory for the foundWeights and foundStates vectors
	foundStates.reserve(m_neurons->size());
	foundWeights = m_weights[neuronInQuestion.id()];
	std::for_each(m_neurons->begin(), m_neurons->end(), [&](Neuron& neuron) {foundStates.emplace_back(neuron.getState());});

}



std::vector<Weights> NeuralNetwork::turnMemoriesToWeights(std::vector<std::vector<float>>& memories) {
	/* this function turns a set of memories into a set of weights
	   1. convert each memory into a weight and store all these weights
	   2. get the average of all of these weights
	*/

	//weights  //    row    //     col     // val
	std::vector< Weights > allWeights;
	allWeights.reserve(memories.size());

	// getting all of the weights by converting each memory into a weight set
	for (std::vector<float>& memory : memories) {
		allWeights.emplace_back(convertMemoryToWeights(memory));
	}

	puts("[NeuralNetwork.hpp]: memories turned to weights");


	return allWeights;
}


int NeuralNetwork::neuronToUpdate() {
	// first we check if we need to update the neuron indexes
	if ((m_neuronsIteratedCount == m_neurons->size() - 1) || (m_neuronsIteratedCount == 0)) {
		m_neuronsIteratedCount = 0;
		m_indexes = std::vector<int>(m_neurons->size());
		for (int i = 0; i < m_neurons->size(); i++)
			m_indexes[i] = i;

		// shuffling
		std::shuffle(m_indexes.begin(), m_indexes.end(), std::default_random_engine());
	}

	// getting a random index
    const int new_idx = m_indexes[m_neuronsIteratedCount];
	m_neuronsIteratedCount++;

	return new_idx;

}


// This function updates the states of neurons in the neural network.
// It takes in a reference to the sf::VertexArray object "circles" which stores the positions of the neurons.
void NeuralNetwork::updateStates(sf::VertexArray& circles) {
	// Loop through each tick of the tick speed
	for (int i = 0; i < m_tickspeed; i++) {
		// Get a reference to the neuron to update
		Neuron& neuron = (*m_neurons)[neuronToUpdate()];

		// Create vectors to store the weights and states of the connections of the neuron
		std::vector<float> connectionWeights, connectionStates;

		// Fill these vectors with the weights and states of the connections of the neuron
		getWeightsAndStates(connectionWeights, connectionStates, neuron);

		// Calculate the delta state (change in state) of the neuron using the dot product of the weights and states of its connections
		float deltaState = collapseState(dotProduct(connectionWeights, connectionStates));
		
		// Generate a random noise value within the specified range
		float noise = randfloat(-m_neuronNoiseStrength, m_neuronNoiseStrength);
		
		// Calculate the new state of the neuron by adding the delta state and noise
		float newState = neuron.getState() + deltaState + noise;

		// Clamp the new state to a range of -1 to 1
		if (newState > 1.0f)
			newState = 1.0f;
		else if (newState < -1.0f)
			newState = -1.0f;

		// Set the new state of the neuron
		neuron.setState(circles, newState);

		// Get the position of the neuron
		sf::Vector2f pos = neuron.getPosition(circles);
		// Add random deviation to the position within the specified range
		pos.x += randfloat(-m_neuronPositionDetiriationStrength, m_neuronPositionDetiriationStrength);
		pos.y += randfloat(-m_neuronPositionDetiriationStrength, m_neuronPositionDetiriationStrength);
		// Set the new position of the neuron
		neuron.setPosition(circles, pos);
	}
}


void NeuralNetwork::appendRandomMemory(std::vector<std::vector<float>>& allMemories, const unsigned int amount) {
	/* this generates random memory, mostly used for debugging, generatedMemory is a vector
	   that containes memories (std::vector<float>). and we add a random memory to that vector
	*/
	std::vector<float> memoryToAdd;
	memoryToAdd.reserve(amount);

	for (unsigned int i = 0; i < amount; i++) {
		memoryToAdd.emplace_back(randfloat(-1.0f, 1.0f));
	}

	allMemories.emplace_back(memoryToAdd);
	puts("[NeuralNetwork.hpp]: generated Random Memory");
}


std::vector<Neuron> NeuralNetwork::generateNeurons(int amount, ArrayOfCircles& buffer,
	sf::Color colorOff, sf::Color colorOn, sf::Vector2f possibleStates) {

	std::vector<Neuron> neurons;
	neurons.reserve(amount);

	for (int i = 0; i < amount; i++) {
        const CalculatorNeuron calcNeuron(i, randfloat(-1.0f, 1.0f), possibleStates);
		Neuron neuron(buffer.m_circleArray, calcNeuron, buffer.m_circles[i], colorOff, colorOn);

		neurons.emplace_back(neuron); 
	}

	puts("[NeuralNetwork.hpp]: generated Neurons");

	return neurons;
}


float NeuralNetwork::queryWeight(int neuron1_id, int neuron2_id) const
{
	// gets the weight at the two neuron positions
	return m_weights[neuron1_id][neuron2_id];
}


void NeuralNetwork::initiliseNetwork(std::vector<Neuron>& N_neurons, std::vector< std::vector< float> > allMemories) {
	/* call only once at the start of the program */
	m_neurons = &N_neurons;
	m_weights.reserve(allMemories.size());
	
	std::vector<Weights> weights = turnMemoriesToWeights(allMemories);
	m_weights = mergeWeights(weights).weightsAsVector;
}

void NeuralNetwork::initMemory(std::vector< std::vector<float>> memories) {
	/* call whenever a change in weights is needed */
	std::vector<Weights> weights = turnMemoriesToWeights(memories);
	m_weights = mergeWeights(weights).weightsAsVector;
}

void NeuralNetwork::hardSetWeights(const std::vector<std::vector<float>>& weights) {
	m_weights = weights;
}


float NeuralNetwork::collapseState(float x) const {
	// Collapses the given state to a binary value
	if (x > 0)
		return 0.2f;
	else
		return -0.2f;
}


int NeuralNetwork::getRandomState(sf::Vector2f possibleStates) {
	return (randint(0, 1) == 0) ? (int)possibleStates.x : (int)possibleStates.y;
}


void NeuralNetwork::randomiseMemoryStates(sf::VertexArray& circles) const
{
	for (Neuron& neuron : *m_neurons) {
		neuron.setState(circles, randfloat(-1.0f, 1.0f));
	}
}


void NeuralNetwork::flipTickSpeed() {
	m_tickspeed = (m_tickspeed == 1) ? 100 : 1;
}