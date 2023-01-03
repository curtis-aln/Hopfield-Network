#pragma once

#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>

#include "../neuron_utils/Neuron.hpp"


struct Cell {
	Rect rect{};
	float state = -1;
	std::array<int, 4> indexes = { 0, 1, 2, 3 };

	void setColor(sf::VertexArray& vertexArray) {
		sf::Color color;
		if (state <= 0) color = sf::Color(0, 0, 0);
		else color = sf::Color(255, 255, 255);

		for (int& i : indexes)
			vertexArray[i].color = color;
	}

	void flipState(bool mode) {
		if ((mode == false && state == -1) or (mode == true && state == 1))
			state *= -1;
	}

	void update(sf::VertexArray& renderArray, bool drawMode) {
		flipState(drawMode);

		// updating the colors
		setColor(renderArray);
	}
};



struct Memory {
	Rect border{};
	std::vector<std::vector<Cell>> cells{};
	sf::VertexArray renderArray{};


	// TODO - remove these functions as they arn't very useful
	void init_cells(std::vector<std::vector<Cell>> o_cells) {
		cells = o_cells;
	}


	void init_render(sf::VertexArray o_renderArray) {
		renderArray = o_renderArray;
	}


	void init_border(Rect o_border) {
		border = o_border;
	}


	void draw(sf::RenderWindow& window) {
		window.draw(renderArray);
	}


	Memory copy() {
		return { border, cells, renderArray };
	}


	std::vector<float> memoriesInReadableFormat() {
		std::vector<float> memories;
		memories.reserve(cells.size() * cells[0].size());

		for (int i = 0; i < cells.size(); i++) {
			for (int j = 0; j < cells[i].size(); j++) {
				memories.emplace_back(cells[j][i].state);
			}
		}

		return memories;
	}
};



struct MemoryWrapper {
	Memory memory{};
	Rect relativeBorder{};
	sf::Transform transformer{};

	bool selected = false;
	unsigned int id{};


public:
	MemoryWrapper(Memory _Memory, unsigned int Id, sf::Transform Transformer, Rect RelativeBorder) {

		memory = _Memory;
		id = Id;
		transformer = Transformer;

		// initilising the selected border
		relativeBorder = RelativeBorder;
	}


	bool checkClick(sf::Vector2i mousePosition) {

		if (relativeBorder.checkRectCollision(mousePosition)) {
			selected = not selected;
			return true;
		}
		return false;
	}


	void draw(sf::RenderWindow& window) {
		window.draw(memory.renderArray, transformer);
		if (selected)
			relativeBorder.draw(window);
	}
};
