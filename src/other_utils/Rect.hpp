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
	Rect(const float _x = 0, const float _y = 0, const float _w = 0, const float _h = 0, 
		const sf::Color rectFillColor = { 0, 0, 0, 0 }, const sf::Color rectOutlineColor = { 0, 0, 0, 0 },
		const float rectOutlineWidth = 0) {
		
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
	bool checkRectCollision(const sf::Vector2i& position) const
    {
		if (!(x < position.x) || !(position.x < x + w) || !(y < position.y) || !(position.y < y + h))
			return false;
		return true;
	}

	void setPositionWithRect(const Rect& rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}

	// convinient functions
	sf::Vector2f getPosition() const {
		return { x, y };
	}
	
	sf::Vector2f getSize() const {
		return { w, h };
	}

	// corners
	sf::Vector2f TopLeft() const {
		return { x, y };
	}

	sf::Vector2f BottomLeft() const {
		return { x, y + h };
	}

	sf::Vector2f TopRight() const {
		return { x + w, y };
	}

	sf::Vector2f BottomRight() const {
		return { x + w, y + h };
	}
	
	// renderFunctions
	void draw(sf::RenderWindow& window) const {
		window.draw(renderRect);
	}
};