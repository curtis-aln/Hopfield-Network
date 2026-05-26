#include "Simulation.hpp"

Simulation::Simulation() {
    const ObjectManager<Button> buttonsMain;
	m_buttons.AddObject("main", buttonsMain);

    const ObjectManager<Button> buttonsMemory;
	m_buttons.AddObject("memory", buttonsMemory);

    const ObjectManager<Button> buttonsBoth;
	m_buttons.AddObject("both", buttonsBoth);
}

void Simulation::initNeuronInfo(const Rect& neuralNetworkBorder, const Rect& memoryMakerBorder, const Rect& memoryManagerBorder) {

	// assigning variables to class
	m_neuralNetworkBorder = neuralNetworkBorder;
	m_memoryMakerBorder = memoryMakerBorder;
	m_memoryManagerBorder = memoryManagerBorder;

	// INITILISING EVERYTHING
	initiliseCircles();
	initiliseNeurons();
	initiliseNeuralNetwork();
	initiliseRenderer();
	initiliseMemoryMaker();
	initiliseMemoryManager();
}

void Simulation::initiliseCircles() {
	// first we need to construct a bunch of circles
	m_buffer = ArrayOfCircles(p_neuronAmount, m_neuronRadius, m_neuronShapeSides);
	puts("[Simulation.hpp]: ArrayOfCircles initilised");
}


void Simulation::initiliseNeurons() {
	// next we generate our neurons
	m_neurons = NeuralNetwork::generateNeurons(
		p_neuronAmount, m_buffer, m_neuronColorNegative, m_neuronColorPositive, { -1, 1 });

	puts("[Simulation.hpp]: Neurons initilised");
}


void Simulation::initiliseNeuralNetwork() {
	// next we create the NeuralNetwork passing in the neurons 
	//m_neuralNetwork = NeuralNetwork();

	std::vector<std::vector<float>> memories;
	NeuralNetwork::appendRandomMemory(memories, p_neuronAmount);
	m_neuralNetwork.initiliseNetwork(m_neurons, memories);

	puts("[Simulation.hpp]: NeuralNetwork initilised");
}


void Simulation::initiliseRenderer() {
	// Finally we pass in the neuralNetwork, the neurons, and the buffer to create the Renderer
	//m_renderer = Renderer();
	m_renderer.initiliseRenderer(m_neuralNetwork, p_neuronsPerAxies, m_neuronRadius, m_neuralNetworkBorder, m_buffer.m_circleArray, m_neurons);
	puts("[Simulation.hpp]: Renderer initilised");
}

void Simulation::initiliseMemoryManager() {
	m_memoryManager.init(m_memoryManagerBorder, { 0.15f, 0.15f }, { 4, 2 });
	puts("[Simulation.hpp] MemoryManager initilised:");
}


void Simulation::initiliseMemoryMaker() {
	m_memoryMaker = MemoryMaker(m_memoryMakerBorder, p_neuronsPerAxies, p_neuronsPerAxies);
	puts("[Simulation.hpp]: MemoryMaker initilised");
}