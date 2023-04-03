#pragma once
#include "SFML/Graphics.hpp"

namespace Settings
{
	const sf::Vector2f SPAWN_LOCATION(1200.0f, 480.0f); // Spawn position on map
	const sf::Vector2f BODY_SIZE(20.0f, 20.0f); // Size of each car (preferred square for collision)
	const float VISION_RADIUS = 160.0f; // Vision range in 5 directions
	const float STARTING_ANGLE = 90.0f; // Starting rotation of car ( to fix texture angle )
	const float MAXIMUM_SPEED = 15.0f;  // Maximum speed of cars
	const float SPEED_INCREASE = 0.2f; // How fast car is getting speed
	const float SPEED_DECREASE = 0.3f; // How fast car is losing speed
	const float TURN_FORCE = 1.0f; // How fast car will turn
}

enum class Movement
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
};


class Car
{
public:
	Car();
	void update(Movement m1,Movement m2);
	void resetCar();
	bool checkCollision(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& mapBoundaries);
	std::vector<double> getVision();
	// Getters
	sf::Vector2f getPosition() { return position; }
	double getAngle() { return angle; }
	double getDistance() { return distance; }
	bool isAlive() { return alive; }
private:
	sf::Vector2f position;
	double distance;
	double speed;
	double angle;
	int inactiveCounter;
	bool alive;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> collisionLines;
};