#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>


struct Circle {
public:
	std::vector<unsigned int> m_circleIndexes{};

public:
	Circle(std::vector<unsigned int> circleIndexes) {
		m_circleIndexes = circleIndexes;
	}

	void move(sf::VertexArray& circles, float deltaX, float deltaY) {
		for (unsigned int& index : m_circleIndexes) {
			circles[index].position.x += deltaX;
			circles[index].position.y += deltaY;
		}
	}

	sf::Vector2f getCenter(sf::VertexArray& circles) {
		float sumsX = 0;
		float sumsY = 0;
		float count = 0;
		for (unsigned int& index : m_circleIndexes) {
			sumsX += circles[index].position.x;
			sumsY += circles[index].position.y;
			count++;
		}

		return sf::Vector2f(sumsX / count, sumsY / count);
	}

	void setPosition(sf::VertexArray& circles, float positionX, float positionY) {
		sf::Vector2f centerPos = getCenter(circles);
		float deltaX = positionX - centerPos.x;
		float deltaY = positionY - centerPos.y;

		move(circles, deltaX, deltaY);
	}

	void setColor(sf::VertexArray& circles, sf::Color color) {
		for (unsigned int& index : m_circleIndexes) {
			circles[index].color = color;
		}
	}
};


class ArrayOfCircles {
public:
	std::vector<Circle> m_circles{};
	sf::VertexArray m_circleArray{};

	int pointsPerCircle{};
	float m_size = 10.0f;

private: // preset variables
	unsigned int m_circlePoints{};
	unsigned int m_circleCount{};
	float m_circleRadius{};

public: // constructor and public functions
	ArrayOfCircles(unsigned int circleCount = 0, float circleRadius = 0, unsigned int circlePoints = 0) {
		
		m_circleRadius = circleRadius;
		m_circlePoints = circlePoints;
		m_circleCount = circleCount;

		m_circleArray.setPrimitiveType(sf::Triangles);

		if (circleCount == 0) return;

		// initilising the Circles 
		std::cout << "[ArrayOfCircles]: createCircleTriangles" << "\n";
		std::vector<std::vector<sf::Vector2f>> PointsForCircles = createCircleTriangles();

		std::cout << "[ArrayOfCircles]: flattenAndConvertToVertexArray" << "\n";
		m_circleArray = flattenAndConvertToVertexArray(PointsForCircles);

		std::cout << "[ArrayOfCircles]: convertVertexArrayToCircles" << "\n";
		m_circles = convertVertexArrayToCircles(m_circleArray);
	}

	void drawCircles(sf::RenderWindow& window, sf::Vector2f center) {
		window.draw(m_circleArray);
	}


private: // private functions
	
	std::vector<std::vector<sf::Vector2f>> createCircleTriangles() {
		std::vector<std::vector<sf::Vector2f>> pointsAsTriangles;

		std::vector<sf::Vector2f> trianglePoints = pointsToTriangles({ 0, 0 });

		for (unsigned int i = 0; i < m_circleCount; i++) {
			pointsAsTriangles.push_back(trianglePoints);
		}

		return pointsAsTriangles;
	}

	
	std::vector<sf::Vector2f> pointsToTriangles(sf::Vector2f positionCenter) {
		std::vector<sf::Vector2f> triangles;

		for (int i = 0; i < (int)m_circlePoints; i++) {
			sf::Vector2f positionA = idxToCoords(i);
			sf::Vector2f positionB = idxToCoords(i+1);
			
			triangles.push_back(positionA + positionCenter);
			triangles.push_back(positionB + positionCenter);
			triangles.push_back(positionCenter);
		}

		pointsPerCircle = (int)triangles.size();
		return triangles;
	}

	
	sf::Vector2f idxToCoords(int idx) {
		float pi = 3.141592f;
		float x = cos(2 * pi / m_circlePoints * idx) * m_circleRadius;
		float y = sin(2 * pi / m_circlePoints * idx) * m_circleRadius;
		return sf::Vector2f(x, y);
	}

	
	std::vector<Circle> convertVertexArrayToCircles(sf::VertexArray& vertexArray) {
		std::vector<Circle> circles;

		for (int i = 0; i < vertexArray.getVertexCount(); i+= pointsPerCircle) {
			std::vector<unsigned int> points;

			for (int k = i; k < i + pointsPerCircle; k++) {
				points.push_back(k);
			}
			Circle circle(points);
			circles.push_back(circle);
		}

		return circles;
	}

	sf::VertexArray flattenAndConvertToVertexArray(std::vector<std::vector<sf::Vector2f>>& trianglePoints) {
		sf::VertexArray vertexArray(sf::Triangles, trianglePoints.size() * trianglePoints[0].size());

		int i = 0;
		for (std::vector<sf::Vector2f>& circle : trianglePoints) {
			for (sf::Vector2f& point : circle) {
				vertexArray[i].position = point;
				i++;
			}
		}

		return vertexArray;
	}
	
};