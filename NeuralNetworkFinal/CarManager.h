#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "GeneticAlgorithm.h"

class CarManager
{
public:
	CarManager(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo);

	void draw(sf::RenderTarget& target);
	void update(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& mapBoundaries);
	void reset();

private:
	sf::Texture carTexture;
	std::vector<Car>carVec;
	GeneticAlgorithm genAlgo;
	int simulationTime; // Maximum number of frames for each generation
};