#include "Simulation.hpp"


bool Simulation::buttonPressedInsideVector(std::string buttonArrayName, sf::Vector2i mousePosition) {
	for (Button* button : m_buttons.FindObject(buttonArrayName)->GetValues()) {
		if (button->checkClick(mousePosition) == true) {

			// specific cases
			if (button->getStrText() == "Add")
				addMemory();

			else if (button->getStrText() == "Remove")
				m_memoryManager.deleteSelected();

			else if (button->getStrText() == "Load")
				loadMemory();

			return true;
		}
	}
	return false;
}


void Simulation::eventCheck(sf::Event& event, sf::RenderWindow& window, sf::Vector2i mousePosition) {
	m_mousePosition = mousePosition;

	while (window.pollEvent(event)) {
		bool result = false;
		result = handleCloseEvent(window, event);
		if (result) break;
		result = handleKeyPressEvent(window, event);
		if (result) break;
		result = handleMouseButtonPressEvent(event, mousePosition);
		if (result) break;
		result = handleMouseButtonReleaseEvent(event);
		if (result) break;
	}
}

bool Simulation::handleCloseEvent(sf::RenderWindow& window, const sf::Event& event) {
	if (event.type == sf::Event::Closed) {
		window.close();
		return true;
	}
	return false;
}

bool Simulation::handleKeyPressEvent(sf::RenderWindow& window, const sf::Event& event) {
	bool interacted = false;
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			window.close();
			interacted = true;
			break;
		case sf::Keyboard::R:
			m_neuralNetwork.randomiseMemoryStates(m_buffer.m_circleArray);
			interacted = true;
			break;
		case sf::Keyboard::C:
			m_memoryMaker.reset();
			interacted = true;
			break;
		case sf::Keyboard::I:
			m_memoryMaker.invert();
			interacted = true;
			break;
		case sf::Keyboard::Space:
			m_paused = not m_paused;
			interacted = true;
			break;
		case sf::Keyboard::S:
			m_neuralNetwork.flipTickSpeed();
			interacted = true;
			break;
		default:
			break;
		}
	}

	return interacted;
}


bool Simulation::handleMouseButtonPressEvent(const sf::Event& event, sf::Vector2i mousePosition) {
	if (event.type == sf::Event::MouseButtonPressed) {
		m_mouseCurrentlyPressed = true;

		if (getButton("both", "Neural Net")->getClicked()) { // main screen
			if (buttonPressedInsideVector("main", mousePosition)) // main buttons
				return true;

			else if (getButton("both", "Make memory")->checkClick(mousePosition, false)) { // switch screen
				manageOptionButtons("make memory");
				return true;
			}
		}

		else if (getButton("both", "Make memory")->getClicked()) { // memory maker screen
			if (getButton("both", "Neural net")->checkClick(mousePosition, false)) {
				manageOptionButtons("neural net");
				return true;
			}

			else {
				if (buttonPressedInsideVector("memory", mousePosition)) // memory buttons
					return true;
			}
		}

		if (m_memoryManager.checkClick(mousePosition)) // memory manager
			return true;
	}

	return false;
}


bool Simulation::handleMouseButtonReleaseEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonReleased) {
		m_mouseCurrentlyPressed = false;
		return true;
	}
	return false;
}


// wrapper function
bool Simulation::checkMemoryClick(sf::Vector2i mousePosition, bool clicked, bool drawMode, bool drawSize) {
	return m_memoryMaker.checkClick(mousePosition, clicked, drawMode, drawSize);
}



void Simulation::memoryMakerClickCheck() {
	bool buttonClicked = getButton("both", "make memory")->getClicked();

	if (buttonClicked == true && m_mouseCurrentlyPressed == true) {

		bool drawMode = getButton("memory", "Add/Remove")->getClicked();
		bool drawSize = getButton("memory", "Brush Size")->getClicked();
		checkMemoryClick(m_mousePosition, true, drawMode, drawSize);
	}
}


void Simulation::manageOptionButtons(std::string name) {

	for (Button* button : m_buttons.FindObject("both")->GetValues()) {
		button->setClicked(false);
	}

	getButton("both", name)->setClicked(true);
}
