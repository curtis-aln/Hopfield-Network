#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct Rect {
public: // variables
	float x{};
	float y{};
	float w{};
	float h{};

	sf::RectangleShape renderRect{};

public:
	// constructor
	Rect(float _x = 0, float _y = 0, float _w = 0, float _h = 0, 
		sf::Color rectFillColor = { 0, 0, 0, 0 }, sf::Color rectOutlineColor = { 0, 0, 0, 0 },
		float rectOutlineWidth = 0) {
		
		x = _x;
		y = _y;
		w = _w;
		h = _h;

		// initilising renderRect
		renderRect.setPosition(getPosition());
		renderRect.setSize(getSize());
		renderRect.setFillColor(rectFillColor);
		renderRect.setOutlineColor(rectOutlineColor);
		renderRect.setOutlineThickness(rectOutlineWidth);
	}

	// other
	bool checkRectCollision(sf::Vector2i position) {
		if (!(x < position.x) || !(position.x < x + w) || !(y < position.y) || !(position.y < y + h))
			return false;
		return true;
	}

	void setPositionWithRect(Rect rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	// convinient functions
	sf::Vector2f getPosition() {
		return { x, y };
	}
	
	sf::Vector2f getSize() {
		return { w, h };
	}

	// corners
	sf::Vector2f TopLeft() {
		return { x, y };
	}

	sf::Vector2f BottomLeft() {
		return { x, y + h };
	}

	sf::Vector2f TopRight() {
		return { x + w, y };
	}

	sf::Vector2f BottomRight() {
		return { x + w, y + h };
	}
	
	// renderFunctions
	void draw(sf::RenderWindow& window) {
		window.draw(renderRect);
	}
};