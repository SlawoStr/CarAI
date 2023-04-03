#include "Simulation.h"
#include <chrono>
#include <iostream>

Simulation::Simulation() : MovingWindow(sf::Vector2u(1920, 1080), "Simulation", sf::Style::Default, 1000), manager({ std::make_tuple("Input Layer", 6, ReluFunction),std::make_tuple("Layer 1",6,ReluFunction),std::make_tuple("Layer 2", 6, SigmoidFunction),std::make_tuple("OutputLayer", 2, nullptr) })
{
	// Load borad texture
	if (!texture.loadFromFile("images/Track.png"))
	{
		std::cout << "Track file doesnt exist" << std::endl;
	}
}

void Simulation::run()
{
	while (window.isOpen())
	{
		//auto start = std::chrono::steady_clock::now();
		window.clear(sf::Color(128, 128, 128));
		draw();
		window.display();
		if (!pause)
		{
			update();
		}
		pollEvent();
		//auto end = std::chrono::steady_clock::now();
		//std::cout << "Elapsed time in miliseconds: "
		//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		//	<< " ms" << std::endl;
	}
}

void Simulation::update()
{
	manager.update(gameMap.getBoundaries());
}

void Simulation::draw()
{
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(5000.0f, 5000.f));
	shape.setTexture(&texture);
	window.draw(shape);
	gameMap.draw(window);
	// Optional - draw boundary lines on game background
	manager.draw(window);
}

void Simulation::pollEvent()
{
	sf::Event e;

	while (window.pollEvent(e))
	{
		if (handleWindowEvent(e))
		{
			continue;
		}
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::R)
			{
				gameMap.restBoundaries();
			}
			else if (e.key.code == sf::Keyboard::S)
			{
				gameMap.saveBoundaries();
			}
			else if (e.key.code == sf::Keyboard::L)
			{
				gameMap.loadBoundaries();
			}
			//Resume game
			else if (e.key.code == sf::Keyboard::P)
			{
				pause = !pause;
			}
		}

		if (e.type == sf::Event::MouseButtonPressed)
		{
			// Set up startingPosition for line
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			startPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		}
		// Set up end position for line
		else if (e.mouseButton.button == sf::Mouse::Right)
		{
			endPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		}
		}
	}
	// Only if both line position Vector are set up create new line and reset variables
	if ((startPos.x != 0 || startPos.y != 0) && (endPos.x != 0 || endPos.y != 0))
	{
		gameMap.addBoundary(std::make_pair(startPos, endPos));
		startPos = sf::Vector2f(0.0f, 0.0f);
		endPos = sf::Vector2f(0.0f, 0.0f);
	}
}
