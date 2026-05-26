#include <SFML/Graphics.hpp>

#include "../connectives/Simulation/Simulation.hpp"
#include "../screen_utils/ScreenText.hpp"

/*
TODO: prep for v2.0
    - fix up memory manager
    - make sure there are no lag spikes
    - button texts aren't aligned
    - replace weights with synapses
*/


// ─── Initialisation ───────────────────────────────────────────────────────────

void init_statistics(
    const float bufferX,
    const float bufferY,
    const sf::Font& font,
    const Simulation& simulation,
    ObjectManager<StatisticsManager>& statistics)
{
    StatisticsManager stats;
    stats.init(font, 20);

    statistics.AddObject("main", stats);
    statistics.AddObject("memory", stats);
    statistics.AddObject("both", stats);

    // Persistent stats (always visible)
    statistics.FindObject("both")->addText("MS/F: ", { 80, 245 });
    statistics.FindObject("both")->addText("Neurons: ", { 80, 270 });
    statistics.FindObject("both")->addText("Per Axies: ", { 80, 295 });
    statistics.FindObject("both")->updateStatistic(std::to_string((float)simulation.p_neuronAmount), 1);
    statistics.FindObject("both")->updateStatistic(std::to_string((float)simulation.p_neuronsPerAxies), 2);

    // Screen-specific key hints
    const float hintY = bufferY + 640;

    statistics.FindObject("main")->addText("P: Pause", { 720, hintY });
    statistics.FindObject("main")->addText("| R: Randomise", { 800, hintY });
    statistics.FindObject("main")->addText("| D: Draw Weights", { 930, hintY });

    statistics.FindObject("memory")->addText("C: Clear All", { 750, hintY });
    statistics.FindObject("memory")->addText("| I: Invert", { 840, hintY });
}


void init_buttons(float minY, float spacing, sf::Vector2f buttonSize, Simulation& simulation)
{
    constexpr float x_pos = 90;

    // Main screen buttons
    simulation.addButton("Pause", { x_pos, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "main", true);
    simulation.addButton("Draw Border", { x_pos, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "main", true);

    // Memory screen buttons
    simulation.addButton("Add/Remove", { x_pos, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "memory", true);
    simulation.addButton("Brush Size", { x_pos, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "memory", true);

    // Screen-switch buttons (always visible)
    const float switchX = x_pos + buttonSize.x + 30;
    simulation.addButton("Make Memory", { switchX, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "both", true);
    simulation.addButton("Neural Net", { switchX, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "both", true);

    // Action buttons
    simulation.addButton("Add", { 100, 640, buttonSize.x, buttonSize.y }, "memory", false);
    simulation.addButton("Remove", { 280, 640, buttonSize.x, buttonSize.y }, "memory", false);
    simulation.addButton("Load", { 140, 640, buttonSize.x, buttonSize.y }, "main", false);

    simulation.getButton("both", "Neural Net")->flipClicked();
}


// ─── Entry Point ─────────────────────────────────────────────────────────────

int main()
{
    // Window / performance settings
    constexpr unsigned int screenWidth = 1366;
    constexpr unsigned int screenHeight = 768;
    constexpr unsigned int FPS_Limit = 99999;
    constexpr bool         b_vSync = false;

    // Layout constants
    constexpr float bufferX = 70;
    constexpr float bufferY = 50;

    constexpr float    minButtonPosY = 70;
    const sf::Vector2f buttonSize{ 150.0f, 40.0f };
    const float        buttonSpacing = buttonSize.y + 10;

    // UI surfaces
    static const std::vector<Rect> surfaces = {
        { 630,        bufferY,           screenHeight - (bufferY * 2), screenHeight - (bufferY * 2), { 35, 36, 54 } }, // neural net
        { bufferX,    bufferY,           400, 150, { 35, 36, 54 } }, // buttons
        { bufferX,    bufferY + 180,     400, 150, { 35, 36, 54 } }, // stats
        { bufferX,    bufferY + 360,     400, 300, { 35, 36, 54 } }, // memory manager
    };

    // Borders
    const float borderSize = surfaces.at(0).getSize().y - 60;
    const Rect neuralNetworkBorder{ 660.0f, bufferY + 30.0f, borderSize, borderSize };
    const Rect memoryMakerBorder{ 660.0f, bufferY + 30.0f, borderSize, borderSize, { 0, 0, 0, 0 }, { 255, 255, 255 }, 3 };
    const Rect memoryManagerBorder{ bufferX - 210, bufferY + 2760, 400, 300 };


    // ── Setup ────────────────────────────────────────────────────────────────

    Simulation simulation;
    simulation.initNeuronInfo(neuralNetworkBorder, memoryMakerBorder, memoryManagerBorder);

    sf::Font font;
    StatisticsManager::createFont(font, "media/Calibri.ttf");

    ObjectManager<StatisticsManager> statistics;
    init_statistics(bufferX, bufferY, font, simulation, statistics);
    init_buttons(minButtonPosY, buttonSpacing, buttonSize, simulation);

    sf::RenderWindow window(sf::VideoMode{ {screenWidth, screenHeight} }, "", sf::Style::Titlebar);
    window.setFramerateLimit(FPS_Limit);
    window.setVerticalSyncEnabled(b_vSync);
    window.resetGLStates();

    sf::Clock frameClock;
    sf::Clock fpsDisplayClock;


    // ── Main Loop ─────────────────────────────────────────────────────────────

    while (window.isOpen())
    {
        // Timing
        const float deltaTime = frameClock.restart().asSeconds();
        const float fps = 1.0f / deltaTime;

        if (fpsDisplayClock.getElapsedTime().asMilliseconds() >= 100)
        {
            statistics.FindObject("both")->updateStatistic(std::to_string(fps), 0);
            fpsDisplayClock.restart();
        }

        // Events
        const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        simulation.processEvents(window, mousePosition);

        // Render
        window.clear({ 15, 15, 31 });

        for (const Rect& surface : surfaces)
            surface.draw(window);

        statistics.FindObject("both")->drawAllText(window);

        if (simulation.getButton("both", "Neural Net")->getClicked())
            statistics.FindObject("main")->drawAllText(window);
        else
            statistics.FindObject("memory")->drawAllText(window);

        simulation.updateAndRender(window);

        window.display();
    }

    return 0;
}