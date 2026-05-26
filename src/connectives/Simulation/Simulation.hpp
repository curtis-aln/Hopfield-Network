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
    inline static constexpr unsigned int p_neuronsPerAxies = 70;
    inline static constexpr unsigned int p_neuronAmount = p_neuronsPerAxies * p_neuronsPerAxies;


    // ─── Member Variables ─────────────────────────────────────────────────────

private:

    // State
    bool m_paused = false;
    bool m_mouseCurrentlyPressed = false;
    sf::Vector2i m_mousePosition;

    // Constants
    inline static constexpr unsigned int m_neuronShapeSides = 5;
    inline static constexpr unsigned int m_button_char_size = 20;
    inline static constexpr float        m_neuronRadius = 5.0f;

    // Colors
    inline static const sf::Color m_neuronColorNegative{ 0,   0,   0 };
    inline static const sf::Color m_neuronColorPositive{ 205, 205, 255 };
    inline static const sf::Color m_buttonColorOff{ 30,  30,  30 };
    inline static const sf::Color m_buttonColorOn{ 20,  100, 122 };

    // Borders
    Rect m_neuralNetworkBorder;
    Rect m_memoryMakerBorder;
    Rect m_memoryManagerBorder;

    // Containers
    ObjectManager< ObjectManager<Button> > m_buttons;
    std::vector<Neuron> m_neurons;

    // Core components
    ArrayOfCircles m_buffer;
    NeuralNetwork  m_neuralNetwork;
    Renderer       m_renderer;
    MemoryMaker    m_memoryMaker;
    MemoryManager  m_memoryManager;

    // Resources
    sf::Font m_font;


    // ─── Private Methods ──────────────────────────────────────────────────────

public:

    // Initialisation
    void initiliseCircles();
    void initiliseNeurons();
    void initiliseNeuralNetwork();
    void initiliseRenderer();
    void initiliseMemoryManager();
    void initiliseMemoryMaker();

    // Event entry point
    void processEvents(sf::RenderWindow& window, sf::Vector2i mousePos);

    // Event handlers
    bool onWindowClose(sf::RenderWindow& window, const sf::Event& event);
    bool onKeyPress(sf::RenderWindow& window, const sf::Event& event);
    bool onMousePress(const sf::Event& event, sf::Vector2i mousePos);
    bool onMouseRelease(const sf::Event& event);

    // Button helpers
    bool handleButtonsInGroup(const std::string& groupName, sf::Vector2i mousePos);
    void setActiveScreen(const std::string& screenName);

    // Memory maker input
    bool tryMemoryMakerClick(sf::Vector2i mousePos, bool clicked, bool drawMode, bool largeBrush);
    void tickMemoryMakerInput();

    // Memory actions
    void addMemory();
    void loadMemory();

    // Learning
    void update_weights(int numNeurons, const double* outputs, double** weights, double learningRate);
    std::vector<double> storkey_learning_rule(const std::vector<double>& weights,
        const std::vector<double>& inputs,
        const double& error,
        const double& learningRate);


    // ─── Public Methods ───────────────────────────────────────────────────────

public:

    Simulation();

    void initNeuronInfo(const Rect& neuralNetworkBorder,
        const Rect& memoryMakerBorder,
        const Rect& memoryManagerBorder);

    // Button management
    void    addButton(const std::string& text, const Rect& rect,
        const std::string& addToList, bool isToggleButton);
    Button* getButton(const std::string& buttonList, const std::string& buttonName);

    // Main loop
    void updateAndRender(sf::RenderWindow& window);
};