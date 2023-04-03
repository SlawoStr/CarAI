#pragma once
#include "MovingWindow.h"
#include "CarManager.h"
#include "GameMap.h"


class Simulation : public MovingWindow
{
public:
	Simulation();
	void run() override;
private:
	void update() override;
	void draw() override;
	void pollEvent() override;
private:
	bool pause{ true };
	// Track background texture
	sf::Texture texture;
	CarManager manager;
	GameMap gameMap;

	sf::Vector2f startPos;
	sf::Vector2f endPos;
};