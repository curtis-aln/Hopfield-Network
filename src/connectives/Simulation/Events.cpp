#include "Simulation.hpp"

// ─── Button Utilities ────────────────────────────────────────────────────────

bool Simulation::handleButtonsInGroup(const std::string& groupName, sf::Vector2i mousePos) {
    for (Button* button : m_buttons.FindObject(groupName)->GetValues()) {
        if (!button->checkClick(mousePos))
            continue;

        const std::string& label = button->getStrText();

        if (label == "Add")    
            addMemory();

        else if (label == "Remove") 
            m_memoryManager.delete_selected();

        else if (label == "Load")   
            loadMemory();

        return true;
    }
    return false;
}

void Simulation::setActiveScreen(const std::string& screenName) {
    for (Button* button : m_buttons.FindObject("both")->GetValues())
        button->setClicked(false);

    getButton("both", screenName)->setClicked(true);
}

// ─── Event Entry Point ───────────────────────────────────────────────────────

void Simulation::processEvents(sf::RenderWindow& window, sf::Vector2i mousePos) {
    m_mousePosition = mousePos;

    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (onWindowClose(window, *event))  break;
        if (onKeyPress(window, *event))     break;
        if (onMousePress(*event, mousePos)) break;
        if (onMouseRelease(*event))         break;
    }
}

// ─── Event Handlers ──────────────────────────────────────────────────────────

bool Simulation::onWindowClose(sf::RenderWindow& window, const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        window.close();
        return true;
    }
    return false;
}

bool Simulation::onKeyPress(sf::RenderWindow& window, const sf::Event& event) {
    const auto* keyEvent = event.getIf<sf::Event::KeyPressed>();
    if (!keyEvent)
        return false;

    switch (keyEvent->code) {
    case sf::Keyboard::Key::Escape:
        window.close();
        return true;

    case sf::Keyboard::Key::R:
        m_neuralNetwork.randomiseMemoryStates(m_buffer.m_circleArray);
        return true;

    case sf::Keyboard::Key::C:
        m_memoryMaker.reset();
        return true;

    case sf::Keyboard::Key::I:
        m_memoryMaker.invert();
        return true;

    case sf::Keyboard::Key::Space:
        m_paused = !m_paused;
        return true;

    case sf::Keyboard::Key::S:
        m_neuralNetwork.flipTickSpeed();
        return true;

    default:
        return false;
    }
}

bool Simulation::onMousePress(const sf::Event& event, sf::Vector2i mousePos) {
    const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
    if (!mouseEvent)
        return false;

    m_mouseCurrentlyPressed = true;

    // Main screen
    if (getButton("both", "Neural Net")->getClicked())
    {
        // main buttons
        if (handleButtonsInGroup("main", mousePos))
            return true;

        // switch between screens
        if (getButton("both", "make memory")->checkClick(mousePos, false)) 
        {
            setActiveScreen("make memory");
            return true;
        }
    }

    // memory maker screen
    else if (getButton("both", "make memory")->getClicked())                
    {
        // switch between screens
        if (getButton("both", "Neural Net")->checkClick(mousePos, false))  
        {
            setActiveScreen("Neural Net");                                
            return true;
        }

        // memory buttons
        if (handleButtonsInGroup("memory", mousePos))
            return true;
    }

    if (m_memoryManager.handle_click(mousePos))
        return true;

    return false;
}

bool Simulation::onMouseRelease(const sf::Event& event) {
    if (event.is<sf::Event::MouseButtonReleased>()) {
        m_mouseCurrentlyPressed = false;
        return true;
    }
    return false;
}

// ─── Memory Maker ────────────────────────────────────────────────────────────

bool Simulation::tryMemoryMakerClick(sf::Vector2i mousePos, bool clicked, bool drawMode, bool largebrush) {
    return m_memoryMaker.handle_click(mousePos, clicked, drawMode, largebrush);
}

void Simulation::tickMemoryMakerInput() {
    const bool onMemoryScreen = getButton("both", "make memory")->getClicked();

    if (onMemoryScreen && m_mouseCurrentlyPressed) {
        const bool drawMode = getButton("memory", "Add/Remove")->getClicked();
        const bool largeBrush = getButton("memory", "Brush Size")->getClicked();
        tryMemoryMakerClick(m_mousePosition, true, drawMode, largeBrush);
    }
}