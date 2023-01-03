#pragma once

#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "../memory_utils/Memory.hpp"

class MemoryMaker {
private:
	Memory m_memory{};
	Rect m_border{};

	float m_cellWidth{};
	float m_cellHeight{};
	int m_cellsX{};
	int m_cellsY{};


private:
	void initGrid() {
		m_cellWidth = m_memory.border.w / m_cellsX;
		m_cellHeight = m_memory.border.h / m_cellsY;

		std::vector<std::vector<Cell>> cells{};

		for (int x = 0; x < m_cellsX; x++) {
			std::vector<Cell> row;
			for (int y = 0; y < m_cellsY; y++) {
				float pos_x = m_memory.border.x + (x * m_cellWidth);
				float pos_y = m_memory.border.y + (y * m_cellHeight);

				row.push_back({ {pos_x, pos_y, m_cellWidth, m_cellHeight}, -1.0f });
			}
			cells.push_back(row);
		}

		m_memory.init_cells(cells);

	}


	void initRenderArray() {
		// call function after the "initGrid()" function

		sf::VertexArray renderArray(sf::Quads);
		int index = 0;

		for (std::vector<Cell>& row : m_memory.cells) {
			for (Cell& cell : row) {
				renderArray.append({ { cell.rect.x, cell.rect.y } });
				renderArray.append({ { cell.rect.x + cell.rect.w, cell.rect.y } });
				renderArray.append({ { cell.rect.x + cell.rect.w, cell.rect.y + cell.rect.h } });
				renderArray.append({ { cell.rect.x, cell.rect.y + +cell.rect.h } });

				cell.indexes = { index, index + 1, index + 2, index + 3 };
				cell.setColor(renderArray);

				index += 4;
			}
		}

		m_memory.init_render(renderArray);
	}

	bool isValidIndex(int indexX, int indexY) {
		if (indexX < 0 || indexY < 0 || indexX > m_memory.cells.size() - 1 || indexY > m_memory.cells[0].size() - 1)
			return false;
		return true;
	}


	// Updates the state of cells within a certain distance from the current cell
	void updateCellsAroundCell(int centerX, int centerY, bool drawmode, int distance) {
		// Iterate over a range of indices around the current cell
		for (int x = centerX - distance; x <= centerX + distance; ++x) {
			for (int y = centerY - distance; y <= centerY + distance; ++y) {
				// Check if the indices are valid
				if (isValidIndex(x, y)) {
					// Update the state of the cell at the current indices
					m_memory.cells[x][y].update(m_memory.renderArray, drawmode);
				}
			}
		}
	}

	// Updates the state of the cell at the given indices if the indices are valid
	void updateCellIfValid(int x, int y, bool drawmode) {
		if (isValidIndex(x, y)) {
			m_memory.cells[x][y].update(m_memory.renderArray, drawmode);
		}
	}





public:
	MemoryMaker(Rect border = Rect{}, int cellsX = 10, int cellsY = 10) {
		m_cellsX = cellsX;
		m_cellsY = cellsY;
		m_memory.init_border(border);
		m_border = border;

		// creating the Grid and RenderArray
		initGrid();
		initRenderArray();
	}


	void draw(sf::RenderWindow& window) {
		// draws the renderArray to the window
		window.draw(m_memory.renderArray);
	}


	void drawTransform(sf::RenderWindow& window, sf::Transform transform) {
		window.draw(m_memory.renderArray, transform);
	}


	void reset() {
		// sets all of the memories to -1
		for (std::vector<Cell>& row : m_memory.cells) {
			for (Cell& cell : row) {
				cell.state = -1;
				cell.setColor(m_memory.renderArray);
			}
		}
	}


	std::vector<std::vector<Cell>> reverse2dArray(std::vector<std::vector<Cell>> v_array) {
		std::vector<std::vector<Cell>> new_array;
		

		for (size_t i = v_array.size() - 1; i >= -1 ; i--) {
			std::vector<Cell> row;
			for (size_t j = v_array[i].size() - 1; j >= -1; j--) {
				row.push_back(v_array[i][j]);
			}
			new_array.push_back(row);
		}

		return new_array;
	}


	Memory getMemory() {
		return m_memory.copy();
	}


	void invert() {
		// inverts every neuron state inside of the memory
		for (std::vector<Cell>& row : m_memory.cells) {
			for (Cell& cell : row) {
				cell.state *= -1;
				cell.setColor(m_memory.renderArray);
			}
		}
	}


	bool checkClick(sf::Vector2i mousePosition, bool clicked, bool drawMode, bool drawSize) {
		// checking if the position is inside out rect
		if (!m_memory.border.checkRectCollision(mousePosition))
			return false;

		else if (clicked == false)
			return true;

		// mapping the mouse position to a grid index
		int x = (int)((mousePosition.x - m_memory.border.x) / m_cellWidth);
		int y = (int)((mousePosition.y - m_memory.border.y) / m_cellHeight);

		m_memory.cells[x][y].flipState(drawMode);
		m_memory.cells[x][y].setColor(m_memory.renderArray);

		// flipping the state of the cell at that index
		if (drawSize == true)
			updateCellsAroundCell(x, y, drawMode, 5);
		
		return true;
	}


	void drawBorder(sf::RenderWindow& window) {
		m_border.draw(window);
	}
};