#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include "../neuron_utils/Neuron.hpp"

#include <string>
#include <array>


struct ButtonTimer {
	unsigned int timeInterval = 100;
	unsigned int timeRemaining = timeInterval;

	bool getButtonState(bool buttonClicked) {
		/* returns what state the button should be */

		if (buttonClicked == false) {
			timeRemaining = timeInterval;
			return false;
		}

		if (timeRemaining <= 0) {
			timeRemaining = timeInterval;
			return false;
		}
		else {
			timeRemaining -= 1;
			return true;
		}
	}
};


class Button {
	Rect m_rectRenderOff{};
	Rect m_rectRenderOn{};
	ButtonTimer m_buttonTimer{};
	sf::Text m_renderText{};
	std::string m_strText{};

	bool m_toggleButton{};
	bool m_clicked = false;
	

private:
	void nonToggleButtonUpdate() {
		m_clicked = m_buttonTimer.getButtonState(m_clicked);
	}


public:
	void init(Rect rect, sf::Color colorOff, sf::Color colorOn, sf::Text& text, std::string text_content, bool toggleButton = true) {
		m_toggleButton = toggleButton;
		m_strText = text_content;
		m_renderText = text;

		// initilising Rects
		m_rectRenderOff = Rect(rect.x, rect.y, rect.w, rect.h, colorOff);
		m_rectRenderOn = Rect(rect.x, rect.y, rect.w, rect.h, colorOn);

		// text initilisation
		m_renderText.setFillColor(sf::Color(255, 255, 255, 255));
		m_renderText.setPosition({ rect.x, rect.y });
		m_renderText.setString(text_content);
	}


	bool checkClick(sf::Vector2i mousePosition, bool mouseClicked = true) {
		if (m_rectRenderOn.checkRectCollision(mousePosition)) {
			if (mouseClicked)
				flipClicked();
			return true;
		}
		return false;
	}


	void flipClicked() {
		m_clicked = not m_clicked;
	}
	

	void setClicked(bool clicked) {
		m_clicked = clicked;
	}

	bool getClicked() {
		return m_clicked;
	}

	std::string getStrText() {
		return m_strText;
	}


	void draw(sf::RenderWindow& window) {
		if (!m_toggleButton)
			nonToggleButtonUpdate();

		if (m_clicked)
			m_rectRenderOn.draw(window);
		else
			m_rectRenderOff.draw(window);

		// drawing text
		window.draw(m_renderText);
	}

	static sf::Text createText(sf::Font& font, std::string name, int size, std::string fileLocation) {
		if (!font.loadFromFile(fileLocation)) {
			std::cout << "[ERROR]: failed to load font" << "\n";
		}
		sf::Text text(name, font, size);

		text.setStyle(sf::Text::Regular);
		return text;
	}
};
