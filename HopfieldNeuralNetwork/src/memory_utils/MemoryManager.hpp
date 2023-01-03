#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../neuron_utils/Neuron.hpp"
#include "../memory_utils/MemoryMaker.hpp"

/*
The MemoryManager header file in incharge of managing memory passed to it by MemoryMaker.
This file will:
- store memories that can be easily written and read from
- merge memories into a Hofield-Readable format
- render memories into a small rect
*/

struct PotentialMemoryData {
	int id{};
	sf::Transform transformer{};
	sf::Vector2f position{};
};


class MemoryManager {
	Rect m_border{};
	Rect m_borderNormal{};

	std::vector<MemoryWrapper> m_memories{};

	std::vector<PotentialMemoryData> m_predata{};

	sf::Vector2f m_scale{};
	sf::Vector2f m_memory_amounts{};


private:
	void initPreData(sf::Vector2f minPos, sf::Vector2f amounts, sf::Vector2f spacings) {
		int id = 0;
		for (int i = 0; i < amounts.x; i++) {

			for (int j = 0; j < amounts.y; j++) {

				sf::Vector2f position(minPos.x + (i * spacings.x), minPos.y + (j * spacings.y));
				m_predata.push_back({ id, getTransformer(position), position });

				id++;
			}
		}
	}


	sf::Vector2f scaleVector2f(sf::Vector2f scale, sf::Vector2f vector, bool reverse = false) {
		if (reverse == true)
			return { vector.x / scale.x, vector.y / scale.y };
		return { vector.x * scale.x, vector.y * scale.y };
	}


	sf::Transform getTransformer(sf::Vector2f position) {
		sf::Transform transformer;
		sf::Vector2f scaledPosition = scaleVector2f(m_scale, position);
		transformer.translate(scaledPosition);
		transformer.scale(m_scale);

		return transformer;
	}


	Rect getMemoryBorder(int id, Memory memory) {
		// scaling the position and size
		sf::Vector2f scaledPos = scaleVector2f(m_scale, m_predata[id].position);
		sf::Vector2f scaledSize = scaleVector2f(m_scale, memory.border.getSize());

		// adjusting the position and size
		scaledPos.x += scaleVector2f(m_scale, memory.border.getSize()).x + 9;
		scaledPos.y += 13;

		// putting it all together
		Rect memoryBorder(scaledPos.x, scaledPos.y, scaledSize.x, scaledSize.y, { 0, 0, 0, 0 }, { 20, 100, 122 }, 1);
		return memoryBorder;
	}



public:
	MemoryManager() {

	}

	std::vector < std::vector< float > > getMemories() {
		// first we get all of the memories as a float
		std::vector < std::vector< float > > selectedMemories;
		selectedMemories.reserve(m_memories.size());

		for (MemoryWrapper& memory : m_memories) {
			if (memory.selected)
				selectedMemories.emplace_back(memory.memory.memoriesInReadableFormat());
		}
		
		return selectedMemories;
	}


	void init(Rect border, sf::Vector2f scale, sf::Vector2f memory_amounts) {
		m_border = border;
		m_borderNormal = border;
		m_borderNormal.y = scaleVector2f(scale, border.getPosition()).y;
		m_borderNormal.w += 200;

		m_scale = scale;
		m_memory_amounts = memory_amounts;

		initPreData(border.getPosition(), memory_amounts, { 650, 650 });
	}


	bool canAddMemory() {
		// checks if there is space to add memory
		return m_memories.size() < m_predata.size();
	}


	void addMemory(Memory memory) {
		int id = (int)m_memories.size();
		Rect memoryBorder = getMemoryBorder(id, memory);

		// adding the memory and preparing for the next
		MemoryWrapper memoryWapped(memory, id, m_predata[id].transformer, memoryBorder);
		m_memories.push_back(memoryWapped);
	}


	void deleteSelected() {
		// creating an iterator
		for (std::vector<MemoryWrapper>::iterator iter = m_memories.begin(); iter != m_memories.end();) {
			if (iter->selected == true)
				iter = m_memories.erase(iter);
			else
				++iter;
		}
		
		
		// now we have deleted what we want, we need to re-order the array
		for (int id = 0; id < m_memories.size(); id++) {
			m_memories[id].id = id;
			m_memories[id].relativeBorder = getMemoryBorder(id, m_memories[id].memory);
			m_memories[id].transformer = m_predata[id].transformer;
		}
	}


	bool checkClick(sf::Vector2i mousePosition) {
		if (!m_borderNormal.checkRectCollision(mousePosition))
			return false;

		for (MemoryWrapper& memory : m_memories) {
			if (memory.checkClick(mousePosition))
				return true;
		}

		return false;
	}


	void renderMemories(sf::RenderWindow& window) {
		for (MemoryWrapper& memory : m_memories) {
			memory.draw(window);
		}
	}
};