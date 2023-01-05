#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "ScreenText.hpp"
#include "../other_utils/Rect.hpp"
#include "../other_utils/UsefulFunctions/UsefulFunctions.hpp"

class SliderButton {
private:
    // The minimum and maximum values of the slider button
    float m_minValue{};
    float m_maxValue{};

    // The current value of the slider button
    float m_value{};

    // The position of the slider button
    sf::Vector2f m_position{};

    // The colors of the line and dot
    sf::Color m_lineColor{};
    sf::Color m_dotColor{};

    // The line and dot shapes
    sf::RectangleShape m_line{};
    sf::CircleShape m_dot{};

    // Whether the dot is currently being dragged
    bool m_isDragging = false;

    // add a new private member variable for the text display
    sf::Text text{};


public:
    // Constructs a new slider button with the given range and initial value
    SliderButton(sf::Text Value_text, float minValue, float maxValue, float initialValue, sf::Vector2f position, sf::Color lineColor = sf::Color::White, sf::Color dotColor = sf::Color::White)
        : m_minValue(minValue), m_maxValue(maxValue), m_value(initialValue), m_position(position), m_lineColor(lineColor), m_dotColor(dotColor)
    {
        // Create the line and dot shapes
        m_line.setSize(sf::Vector2f(200.0f, 5.0f));
        m_line.setFillColor(lineColor);
        m_line.setPosition(position);

        text = Value_text;

        m_dot.setRadius(10.0f);
        m_dot.setFillColor(dotColor);
        updateDotPosition();
    }


    // Handles input events and updates the slider button state
    void handleInput(const sf::Event& event, sf::Vector2i mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Check if the mouse is over the dot
                if (m_dot.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    m_isDragging = true;
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                m_isDragging = false;
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            if (m_isDragging) {
                // Calculate the new value based on the mouse position
                float mouseX = mousePos.x - m_position.x;
                float range = m_maxValue - m_minValue;
                m_value = m_minValue + range * mouseX / 200.0f;
                m_value = std::clamp(m_value, m_minValue, m_maxValue);
                updateDotPosition();
            }
        }
    }

    // Updates the position of the dot based on the current value
    void updateDotPosition() {
        float range = m_maxValue - m_minValue;
        float percent = (m_value - m_minValue) / range;
        float x = m_position.x + 200.0f * percent;
        float y = m_position.y - 6.0f; // Center the dot vertically
        m_dot.setPosition(x, y);
    }

    // Returns the current value of the slider button
    double getValue() const {
        return m_value;
    }

    // Draws the slider button to the given render target
    void draw(sf::RenderWindow& target, sf::Font& font) {
        target.draw(m_line);
        target.draw(m_dot);

        sf::Vector2f pos = m_dot.getPosition();
        setValueText(std::to_string(getValue()), {pos.x+3, pos.y + 3});
        target.draw(text);
    }

private:
    void setValueText(const std::string& newText, const sf::Vector2f position) {
        text.setString(newText);
        text.setPosition(position);
    }
};
