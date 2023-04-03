#pragma once
#include <SFML/Graphics.hpp>


class MovingWindow
{
public:
	MovingWindow(sf::Vector2u windowSize, sf::String title, sf::Uint32 style = sf::Style::Default, unsigned int framerate = 144);
	virtual void run() = 0;
	bool handleWindowEvent(sf::Event e);
private:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void pollEvent() = 0;
protected:
	std::pair<sf::Vector2f, sf::Vector2f> getViewBox();
protected:
	sf::RenderWindow window;
private:
	float zoomLevel{ 1.0f };
	// Panning
	sf::Vector2i panningAnchor{};
	bool panning{ false };
};