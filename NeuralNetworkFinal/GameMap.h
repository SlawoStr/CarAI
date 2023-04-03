#pragma once
#include "SFML/Graphics.hpp"

class GameMap
{
public:
	void draw(sf::RenderTarget& target);
	// Add new boundary line to vector
	void addBoundary(std::pair<sf::Vector2f, sf::Vector2f> boundary);
	// Load all boundary lines from file
	void loadBoundaries();
	// Save all boundray lines to file
	void saveBoundaries();
	// Delete all boundary lines
	void restBoundaries();
	// Return reference to lines vector
	const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& getBoundaries() { return mapBoundaries; }
private:
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>>mapBoundaries;
};