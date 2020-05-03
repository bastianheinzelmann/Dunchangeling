#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

const sf::Color COLOR_FILLED = sf::Color::Blue;
const sf::Color COLOR_CONFIG_SPACE = sf::Color::Red;
const sf::Color COLOR_CONFIG_FILLED = sf::Color::Cyan;

struct Tile
{
	Tile() : xPosition(0), yPosition(0), colorType(0) {};
	Tile(int x, int y, sf::Color color) : xPosition(x), yPosition(y), colorType(color) {};

	int xPosition;
	int yPosition;

	sf::Color colorType;
};

struct RoomShape
{
	std::vector<Tile> tiles;

	float position = 0;
	float tileSize = 10;

	void DrawRoomShape(sf::RenderWindow& window)
	{
		sf::RectangleShape tileShape(sf::Vector2f(tileSize, tileSize));

		for (auto i : tiles)
		{
			tileShape.setFillColor(i.colorType);
			tileShape.setPosition((i.xPosition * tileSize) + position, (i.yPosition * tileSize) + position);
			window.draw(tileShape);
		}
	}
};