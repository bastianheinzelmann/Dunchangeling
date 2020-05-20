#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"


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

	float PositionX;
	float PositionY;
	float TileSize;

	sf::Color color = sf::Color(randomNumber(0, 255), randomNumber(0, 255), randomNumber(0, 255));

	RoomShape(Grid & grid, int positionX, int positionY, int tileSize)
	{
		PositionX = positionX;
		PositionY = positionY;
		TileSize = tileSize;

		int ySize = grid.YSize;
		int xSize = grid.XSize;

		for (int y = 0; y < ySize; y++)
		{
			for (int x = 0; x < xSize; x++)
			{
				if(grid.Get(x, y) >= GRID_FILLED_NORMAL)
					this->tiles.push_back(Tile(x, y, color));
			}
		}
	}

	void DrawRoomShape(sf::RenderWindow& window)
	{
		sf::RectangleShape tileShape(sf::Vector2f(TileSize, TileSize));

		for (auto i : tiles)
		{
			tileShape.setFillColor(i.colorType);
			tileShape.setPosition((i.xPosition * TileSize) + (PositionX * TileSize), (i.yPosition * TileSize) + (PositionY * TileSize));
			window.draw(tileShape);
		}
	}
};

class LayoutShape
{
public:
	std::vector<RoomShape> RoomShapes;
	int PositionX;
	int PositionY;
	int TileSize;

	LayoutShape(Layout & layout, int positionX, int positionY, int tileSize)
	{
		PositionX = positionX;
		PositionY = positionY;
		TileSize = tileSize;

		for (auto i : layout.Rooms)
		{
			if(i.VertexID != -1)
				RoomShapes.push_back(RoomShape(i.Room.RoomGrid, i.PosX + PositionX, i.PosY + PositionY, tileSize));
		}
	}

	void DrawLayoutShape(sf::RenderWindow& window)
	{
		for (auto i : RoomShapes)
		{
			i.DrawRoomShape(window);
		}
	}
};