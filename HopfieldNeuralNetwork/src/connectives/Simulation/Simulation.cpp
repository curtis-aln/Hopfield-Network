#include "Simulation.hpp"


// button functions
void Simulation::addMemory() {
	// checking if we have space
	if (m_memoryManager.canAddMemory() == false)
		return;

	Memory memory = m_memoryMaker.getMemory();
	m_memoryManager.addMemory(memory);
}


void Simulation::loadMemory() {
	// first we get all of the memories
	std::vector<std::vector<float>> memories = m_memoryManager.getMemories();
	if (memories.size() == 0)
		return;

	// now we convert them into weights
	std::vector<Weights> allWeights = m_neuralNetwork.turnMemoriesToWeights(memories);
	
	// then we merge the memories
	std::vector<std::vector<float>> weights = m_neuralNetwork.mergeWeights(allWeights).weightsAsVector;

	// now we feed it into the neural network
	m_neuralNetwork.hardSetWeights(weights);
}


// button stuff
void Simulation::addButton(std::string text, Rect rect, std::string addToList, bool isToggleButton) {
	sf::Text sf_text = Button::createText(m_font, text, 20, "media/Calibri.ttf");
	Button button;
	button.init(rect, m_buttonColorOff, m_buttonColorOn, sf_text, text, isToggleButton);
	m_buttons.FindObject(addToList)->AddObject(text, button);
}


Button* Simulation::getButton(std::string buttonList, std::string buttonName) {
	return m_buttons.FindObject(buttonList)->FindObject(buttonName);
}


void Simulation::updateAndRender(sf::RenderWindow& window) {
	for (Button* button : m_buttons.FindObject("both")->GetValues()) {
		button->draw(window);
	}
	

	if (getButton("both", "Neural Net")->getClicked()) {
		if (!getButton("main", "pause")->getClicked())
			m_neuralNetwork.updateStates(m_buffer.m_circleArray);

		if (getButton("main", "Draw Border")->getClicked())
			m_memoryMakerBorder.draw(window);

		m_buffer.drawCircles(window, {
			m_neuralNetworkBorder.x + (m_neuralNetworkBorder.w/2), 
			m_neuralNetworkBorder.y + (m_neuralNetworkBorder.h / 2) 
		});

		// drawing all buttons inside of the main page
		for (Button* button : m_buttons.FindObject("main")->GetValues()) {
			button->draw(window);
		}
	}

	else {
		for (Button* button : m_buttons.FindObject("memory")->GetValues()) {
			button->draw(window);
		}

		m_memoryMaker.drawBorder(window);
		m_memoryMaker.draw(window);
	}

	m_memoryManager.renderMemories(window);
}


// Function to update the connection weights in a Hopfield network using the Hebbian learning rule
void Simulation::update_weights(int num_neurons, double* outputs, double** weights, double learning_rate) {
	// Loop through all pairs of neurons
	for (int i = 0; i < num_neurons; i++) {
		for (int j = 0; j < num_neurons; j++) {
			// Calculate the change in the connection weight between neurons i and j using the Hebbian learning rule
			double delta_weight = learning_rate * outputs[i] * outputs[j];
			// Update the connection weight
			weights[i][j] += delta_weight;
		}
	}
}

std::vector<double> Simulation::storkey_learning_rule(std::vector<double> weights, std::vector<double> inputs, double error, double learning_rate) {
	std::vector<double> updated_weights;
	for (int i = 0; i < weights.size(); i++) {
		double weight_update = learning_rate * error * inputs[i];
		updated_weights.push_back(weights[i] + weight_update);
	}
	return updated_weights;
}