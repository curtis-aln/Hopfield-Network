#include <iostream>
#include <SFML/Graphics.hpp>

#include "../connectives/Simulation/Simulation.hpp"
#include "../screen_utils/SliderButton.hpp"

/*
TODO, prep for v2.0
- fix up memory manager
- make sure there are no lag spikes

- button texts arn't aligned
- replace weights with synapses


t1 = 23 seconds
t2 = 
t3 = 
*/



void init_statistics(float bufferX, float bufferY, sf::Vector2f buttonSize, sf::Font& font, Simulation& simulation, ObjectManager<StatisticsManager>& statistics) {
	StatisticsManager stats;
	stats.init(font, 20);
	statistics.AddObject("main", stats);
	statistics.AddObject("memory", stats);
	statistics.AddObject("both", stats);

	statistics.FindObject("both")->addText("MS/F: ", { 80, 245 });
	statistics.FindObject("both")->addText("Neurons: ", { 80, 270 });
	statistics.FindObject("both")->addText("Per Axies: ", { 80, 295 });
	statistics.FindObject("both")->updateStatistic(floatToString((float)simulation.p_neuronAmount), 1);
	statistics.FindObject("both")->updateStatistic(floatToString((float)simulation.p_neuronsPerAxies), 2);

	const float buttonYPos = bufferY + 640;

	statistics.FindObject("main")->addText("P: Pause", { 720, buttonYPos });
	statistics.FindObject("main")->addText("| R: Randomise", { 800, buttonYPos });
	statistics.FindObject("main")->addText("| D: Draw Weights", { 930, buttonYPos });

	statistics.FindObject("memory")->addText("C: Clear All", { 750, buttonYPos });
	statistics.FindObject("memory")->addText("| I: Invert All", { 840, buttonYPos });
}


void init_buttons(float minY, float spacing, sf::Vector2f buttonSize, Simulation& simulation) {
	const float x_pos = 90;
	simulation.addButton("Pause", { x_pos, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "main", true);
	simulation.addButton("Draw Border", { x_pos, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "main", true);

	simulation.addButton("Add/Remove", { x_pos, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "memory", true);
	simulation.addButton("Brush Size", { x_pos, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "memory", true);

	simulation.addButton("Make Memory", { x_pos + buttonSize.x + 30, minY + (spacing * 0), buttonSize.x, buttonSize.y }, "both", true);
	simulation.addButton("Neural Net", { x_pos + buttonSize.x + 30, minY + (spacing * 1), buttonSize.x, buttonSize.y }, "both", true);

	simulation.addButton("Add", { 100, 640, buttonSize.x, buttonSize.y }, "memory", false);
	simulation.addButton("Remove", { 280, 640, buttonSize.x, buttonSize.y }, "memory", false);
	simulation.addButton("Load", { 140, 640, buttonSize.x, buttonSize.y }, "main", false);

	simulation.getButton("both", "Neural Net")->flipClicked();
}



int main() {

	// constants
	const unsigned int screenWidth = 1366;
	const unsigned int screenHeight = 768;
	const unsigned int FPS = 1060;

	const unsigned int SynapsesShown = 1; // how many synapses are shown (eveny Nth synapse)
	const unsigned int frameRateUpdateFreq = 10;

	const float bufferX = 70;
	const float bufferY = 50;

	// - - - - - - - - - - - - - variables - - - - - - - - - - - - - //
	bool neuronWeightDetiriation = false;
	unsigned int framecount = 0;    // current frame


	// graphic design - rectangle everything will be on
	std::vector<Rect> surfaces = {
		{630, bufferY, screenHeight - (bufferY * 2), screenHeight - (bufferY * 2), { 35, 36, 54 }}, // networkSurface
		{bufferX, bufferY, 400, 150, { 35, 36, 54 }}, // buttonSurface
		{bufferX, bufferY + 180, 400, 150, { 35, 36, 54 }}, // statsSurface
		{bufferX, bufferY + 360, 400, 300, { 35, 36, 54 }}, // memorySurface
	};

	// borders
	const float borderSize = surfaces[0].getSize().y - 60;
	const Rect neuralNetworkBorder{ 660.0f, bufferY + 30.0f, borderSize, borderSize };
	const Rect memoryMakerBorder{ 660.0f, bufferY + 30.0f, borderSize, borderSize, { 0, 0, 0, 0 }, { 255, 255, 255 }, 3};
	const Rect memoryManagerBorder{ bufferX - 210, bufferY + 2760, 400, 300 };


	const sf::Vector2i neuronStates(-1, 1);
	const sf::Vector2f buttonSize(150, 40);


	// initilising random
	std::srand(static_cast<unsigned>(time(NULL)));

	const float minButtonPosY = 70;
	const float buttonSpacing = buttonSize.y + 10;


	// initilising simulation
	Simulation simulation;
	simulation.initNeuronInfo(neuralNetworkBorder, memoryMakerBorder, memoryManagerBorder);


	// initilising statistics
	sf::Font font;
	StatisticsManager::createFont(font, "media/Calibri.ttf");
	ObjectManager<StatisticsManager> statistics;
	init_statistics(bufferX, bufferY, buttonSize, font, simulation, statistics);

	// adding buttons
	init_buttons(minButtonPosY, buttonSpacing, buttonSize, simulation);

	// creating the window and the fps manager
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "", sf::Style::Titlebar);
	sf::Clock clock;

	//sf::Text sliderbuttonText("hi", font, 30);
	//sliderbuttonText.setFillColor(sf::Color(255, 255, 255));
	//sliderbuttonText.setPosition(200, 200);
	//sliderbuttonText.setCharacterSize(10);
	//SliderButton sliderButton(sliderbuttonText, 0, 100, 50, { 130, 500 }, { 40, 60, 150 }, {70, 70, 70});

	// main game loop
	while (window.isOpen()) {
		// events
		sf::Event event;
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

		simulation.eventCheck(event, window, mousePosition);
		simulation.memoryMakerClickCheck();

		//sliderButton.handleInput(event, mousePosition);
		//sliderButton.updateDotPosition();
		
		// clearing / resetting the screen
		window.clear({ 15, 15, 31 });

		// drawing the surfaces
		for (Rect& surface : surfaces)
			surface.draw(window);

		// drawing statistics
		statistics.FindObject("both")->drawAllText(window);

		if (simulation.getButton("both", "Neural Net")->getClicked())
			statistics.FindObject("main")->drawAllText(window);
		else
			statistics.FindObject("Memory")->drawAllText(window);
		
		std::string msps = calcFPS(clock);
		if (framecount % frameRateUpdateFreq == 0) {
			statistics.FindObject("both")->updateStatistic(msps, 0);
			framecount = 1;
		}
		framecount++;

		// drawing screen buttons
		simulation.updateAndRender(window);

		//sliderButton.draw(window, font);

		// drawing to screen
		window.display();
	}
}