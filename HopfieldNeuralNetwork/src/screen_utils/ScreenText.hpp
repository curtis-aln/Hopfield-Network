#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

struct Text {
	sf::Text sf_text{};
	std::string std_text{};
	std::string value{};

	Text(sf::Font& font, std::string text_string = "", unsigned int textSize=10, sf::Vector2f position = {0, 0}) {
		sf_text.setStyle(sf::Text::Regular);
		sf_text.setFillColor(sf::Color(255, 255, 255));
		sf_text.setPosition(position);
		sf_text.setFont(font);
		sf_text.setCharacterSize(textSize);

		std_text = text_string;
	}

	void draw(sf::RenderWindow& window) {
		// we join the text and the value with a : before rendering
		sf_text.setString(std_text + value);

		window.draw(sf_text);
	}
};


class StatisticsManager {
	sf::Font m_font;

	std::vector<Text> m_statistics{};

	unsigned int m_textSize{};


public:
	void init(sf::Font& font, unsigned int textSize) {
		m_font = font;
		m_textSize = textSize;
	}

	static void createFont(sf::Font& font, std::string fontFileLocation) {
		if (!font.loadFromFile(fontFileLocation))
			std::cout << "[ERROR]: failed to load font" << "\n";
	}

	void addText(std::string text_string, sf::Vector2f position) {
		m_statistics.push_back({m_font, text_string, m_textSize, position});
	}

	void updateStatistic(std::string value, int index) {
		m_statistics[index].value = value;
	}

	void drawAllText(sf::RenderWindow& window) {
		for (Text& text : m_statistics) {
			text.draw(window);
		}
	}
};