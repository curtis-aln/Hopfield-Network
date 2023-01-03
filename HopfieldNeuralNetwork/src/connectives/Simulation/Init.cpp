#include "Simulation.hpp"

Simulation::Simulation() {
	ObjectManager<Button> buttonsMain;
	m_buttons.AddObject("main", buttonsMain);

	ObjectManager<Button> buttonsMemory;
	m_buttons.AddObject("memory", buttonsMemory);

	ObjectManager<Button> buttonsBoth;
	m_buttons.AddObject("both", buttonsBoth);
}

void Simulation::initNeuronInfo(Rect neuralNetworkBorder, Rect memoryMakerBorder, Rect memoryManagerBorder) {

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
	std::cout << "[Simulation.hpp]: ArrayOfCircles initilised" << "\n";
}


void Simulation::initiliseNeurons() {
	// next we generate our neurons
	m_neurons = NeuralNetwork::generateNeurons(
		p_neuronAmount, m_buffer, m_neuronColorNegative, m_neuronColorPositive, { -1, 1 });

	std::cout << "[Simulation.hpp]: Neurons initilised" << "\n";
}


void Simulation::initiliseNeuralNetwork() {
	// next we create the NeuralNetwork passing in the neurons 
	m_neuralNetwork = NeuralNetwork();

	std::vector<std::vector<float>> memories;
	NeuralNetwork::appendRandomMemory(memories, p_neuronAmount);
	m_neuralNetwork.initiliseNetwork(m_neurons, memories);

	std::cout << "[Simulation.hpp]: NeuralNetwork initilised" << "\n";
}


void Simulation::initiliseRenderer() {
	// Finally we pass in the neuralNetwork, the neurons, and the buffer to create the Renderer
	m_renderer = Renderer();
	m_renderer.initiliseRenderer(m_neuralNetwork, p_neuronsPerAxies, m_neuronRadius, m_neuralNetworkBorder, m_buffer.m_circleArray, m_neurons);
	std::cout << "[Simulation.hpp]: Renderer initilised" << "\n";
}

void Simulation::initiliseMemoryManager() {
	m_memoryManager.init(m_memoryManagerBorder, { 0.15f, 0.15f }, { 4, 2 });
	std::cout << "[Simulation.hpp] MemoryManager initilised:" << "\n";
}


void Simulation::initiliseMemoryMaker() {
	m_memoryMaker = MemoryMaker(m_memoryMakerBorder, p_neuronsPerAxies, p_neuronsPerAxies);
	std::cout << "[Simulation.hpp]: MemoryMaker initilised" << "\n";
}