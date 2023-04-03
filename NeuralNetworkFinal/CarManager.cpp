#include "CarManager.h"
#include <iostream>
#include "GAAlgorithms.h"

CarManager::CarManager(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo) : simulationTime(0), genAlgo(layerInfo, 500)
{
	if (!carTexture.loadFromFile("images/car.png"))
	{
		std::cerr << "Couldnt load car texture" << std::endl;
	}

	genAlgo.setSelection(selectWheel, 460);
	genAlgo.setCrossover(crossoverSPC, 0.8);
	genAlgo.setMutation(mutationADD, 0.8, -0.25, 0.25);

	carVec.reserve(500);
	for (int i = 0; i < 500; i++)
	{
		carVec.push_back(Car());
	}
}

void CarManager::draw(sf::RenderTarget& target)
{
	sf::RectangleShape shape;
	shape.setTexture(&carTexture);
	shape.setSize(sf::Vector2f(Settings::BODY_SIZE));
	shape.setOrigin(Settings::BODY_SIZE.x / 2, Settings::BODY_SIZE.y / 2);

	for (int i = 0; i < carVec.size(); i++)
	{
		shape.setPosition(carVec[i].getPosition().x, carVec[i].getPosition().y);
		shape.setRotation(static_cast<float>(carVec[i].getAngle()));
		if (!carVec[i].isAlive())
		{
			shape.setFillColor(sf::Color::Red);
		}
		else
		{
			shape.setFillColor(sf::Color::Green);
		}
		target.draw(shape);
	}
}

void CarManager::update(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& mapBoundaries)
{
	simulationTime++;

	bool isDead = true;
	for (int i = 0; i < carVec.size(); i++)
	{
		if (carVec[i].isAlive())
		{
			// Get prediction from neural network
			std::vector<double> input = carVec[i].getVision();
			std::vector<double> result = genAlgo.predictOne(i, input);
			Movement m1, m2;

			if (result[0] > 0.5f)
			{
				m1 = Movement::UP;
			}
			else
			{
				m1 = Movement::DOWN;
			}
			if (result[1] > 0.5f)
			{
				m2 = Movement::RIGHT;
			}
			else
			{
				m2 = Movement::LEFT;
			}
			carVec[i].update(m1, m2);
			// Check if any car is still alive
			if (!carVec[i].checkCollision(mapBoundaries))
			{
				isDead = false;
			}
		}
	}
	if (isDead || simulationTime > 7200)
	{
		double bestScore = 0.0f;
		std::vector<std::pair<int, double>> carFitness;
		carFitness.reserve(carVec.size());
		for (int i = 0; i < carVec.size(); i++)
		{
			carFitness.push_back(std::make_pair(i, carVec[i].getDistance()));
			if (carVec[i].getDistance() > bestScore)
			{
				bestScore = carVec[i].getDistance();
			}
		}
		// Best current population score
		std::cout << "Best score: " << bestScore << std::endl;
		// Create new population
		genAlgo.createNewGeneration(carFitness);
		// Reset simulation for next generation
		reset();
	}
}

void CarManager::reset()
{
	simulationTime = 0;

	for (int i = 0; i < carVec.size(); i++)
		carVec[i].resetCar();
}
