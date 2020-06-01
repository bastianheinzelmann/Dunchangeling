#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "../Dunchangeling/Room.h"
#include "../Dunchangeling/Grid.h"
#include "../Dunchangeling/DungeonGrid.h"


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

struct DungeonPart
{
	DungeonPart(sf::Vector2i offset, sf::RectangleShape shape, sf::Color color)
	{
		Offset = offset;
		Shape = shape;
		Color = color;
		Shape.setFillColor(color);
	}

	void Draw(sf::RenderWindow & window, int xPos, int yPos)
	{
		Shape.setPosition(xPos + Offset.x, yPos + Offset.y);
		window.draw(Shape);
	}

	sf::Vector2i Offset;
	sf::RectangleShape Shape;
	sf::Color Color;
};

class FinalGrid
{
public:
	FinalGrid(DungeonGrid & grid)
	{
		dgrid = grid;
	}

	void Draw(sf::RenderWindow & window)
	{
		for (int y = 0; y < dgrid.YSize; y++)
		{
			for (int x = 0; x < dgrid.XSize; x++)
			{
				int tileType = dgrid.Get(x, y, DDE_Tile);
				if (tileType > 0)
				{
					switch (tileType)
					{
					case TILE_FILLED: background.Shape.setFillColor(sf::Color::White); break;
					case TILE_START_ROOM: background.Shape.setFillColor(sf::Color::Green); break;
					case TILE_END_ROOM: background.Shape.setFillColor(sf::Color::Red); break;
					}

					background.Draw(window, x * 16, y * 16);

					switch (dgrid.Get(x, y, DDE_North))
					{
					case 1: wallNorth.Draw(window, x * 16, y * 16); break;
					case 2: doorNorth1.Draw(window, x * 16, y * 16); doorNorth2.Draw(window, x * 16, y * 16); break;
					}

					switch (dgrid.Get(x, y, DDE_East))
					{
					case 1: wallEast.Draw(window, x * 16, y * 16); break;
					case 2: doorEast1.Draw(window, x * 16, y * 16); doorEast2.Draw(window, x * 16, y * 16); break;
					}

					switch (dgrid.Get(x, y, DDE_South))
					{
					case 1: wallSouth.Draw(window, x * 16, y * 16); break;
					case 2: doorSouth1.Draw(window, x * 16, y * 16); doorSouth2.Draw(window, x * 16, y * 16); break;
					}

					switch (dgrid.Get(x, y, DDE_West))
					{
					case 1: wallWest.Draw(window, x * 16, y * 16); break;
					case 2: doorWest1.Draw(window, x * 16, y * 16); doorWest2.Draw(window, x * 16, y * 16); break;
					}
				}
			}
		}
	}

private:
	DungeonGrid dgrid;

	DungeonPart background = DungeonPart( sf::Vector2i(0,0), sf::RectangleShape(sf::Vector2f(16, 16)), sf::Color::White );

	DungeonPart wallNorth = DungeonPart( sf::Vector2i(0,0), sf::RectangleShape(sf::Vector2f(16, 2)), sf::Color::Black );
	DungeonPart wallEast = DungeonPart ( sf::Vector2i(14,0), sf::RectangleShape(sf::Vector2f(2, 16)), sf::Color::Black);
	DungeonPart wallSouth = DungeonPart ( sf::Vector2i(0,14), sf::RectangleShape(sf::Vector2f(16, 2)), sf::Color::Black);
	DungeonPart wallWest = DungeonPart ( sf::Vector2i(0,0), sf::RectangleShape(sf::Vector2f(2, 16)), sf::Color::Black);

	DungeonPart doorNorth1  = DungeonPart ( sf::Vector2i(0,0), sf::RectangleShape(sf::Vector2f(4, 2)), sf::Color::Black);
	DungeonPart doorNorth2  = DungeonPart ( sf::Vector2i(12,0), sf::RectangleShape(sf::Vector2f(4, 2)), sf::Color::Black);
	DungeonPart doorEast1   = DungeonPart ( sf::Vector2i(14,0), sf::RectangleShape(sf::Vector2f(2, 4)), sf::Color::Black);
	DungeonPart doorEast2   = DungeonPart ( sf::Vector2i(14,12), sf::RectangleShape(sf::Vector2f(2, 4)), sf::Color::Black);
	DungeonPart doorSouth1  = DungeonPart ( sf::Vector2i(0,14), sf::RectangleShape(sf::Vector2f(4, 2)), sf::Color::Black);
	DungeonPart doorSouth2  = DungeonPart ( sf::Vector2i(12,14), sf::RectangleShape(sf::Vector2f(4, 2)), sf::Color::Black);
	DungeonPart doorWest1   = DungeonPart ( sf::Vector2i(0,0), sf::RectangleShape(sf::Vector2f(2, 4)), sf::Color::Black);
	DungeonPart doorWest2   = DungeonPart ( sf::Vector2i(0,12), sf::RectangleShape(sf::Vector2f(2, 4)), sf::Color::Black);
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