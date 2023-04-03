#include "GameMap.h"
#include <fstream>
#include <iostream>

void GameMap::draw(sf::RenderTarget& target)
{
	sf::VertexArray BoundaryLines(sf::Lines, mapBoundaries.size() * 2);
	for (int i = 0; i < mapBoundaries.size(); i++)
	{
		BoundaryLines[i * 2].position = mapBoundaries[i].first;
		BoundaryLines[i * 2 + 1].position = mapBoundaries[i].second;
		BoundaryLines[i * 2 + 1].color = sf::Color::Red;
		BoundaryLines[i * 2].color = sf::Color::Red;
	}
	target.draw(BoundaryLines);
}

void GameMap::addBoundary(std::pair<sf::Vector2f, sf::Vector2f> boundary)
{
	mapBoundaries.push_back(boundary);
}

void GameMap::loadBoundaries()
{
	mapBoundaries.clear();
	std::ifstream file;
	file.open("Boundaries.txt", std::ios::in);
	sf::Vector2f startPos, endPos;
	if (!file)
	{
		std::cout << "File doesnt exist" << std::endl;
		return;
	}
	else if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << "Cand load data. File is empty." << std::endl;
	}
	else
	{
		// While file didint end there are atleast 4 another values
		while (!file.eof())
		{
			file >> startPos.x;
			file >> startPos.y;
			file >> endPos.x;
			file >> endPos.y;

			mapBoundaries.push_back(std::make_pair(startPos, endPos));
		}
	}
}

void GameMap::saveBoundaries()
{
	std::ofstream file;
	file.open("Boundaries.txt", std::ios::out | std::ios::trunc);
	for (int i = 0; i < mapBoundaries.size(); i++)
	{
		file << mapBoundaries[i].first.x << " " << mapBoundaries[i].first.y << " ";
		file << mapBoundaries[i].second.x << " " << mapBoundaries[i].second.y << " ";
	}
	file.close();
}

void GameMap::restBoundaries()
{
	mapBoundaries.clear();
}
