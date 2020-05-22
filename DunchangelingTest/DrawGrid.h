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

class FinalGrid
{
public:
	FinalGrid(Grid & grid)
	{
		texture.loadFromFile("C:/Users/Bastian/Documents/MasterStuff/UnityPlugin/Dunchangeling/Sprites/DungeonSheet.png");
		sprite = sf::Sprite(texture);
		spritePosRect = sf::IntRect(0, 0, 16, 16);
		myGrid = grid;
	}

	void Draw(sf::RenderWindow& window)
	{
		for (int y = 0; y < myGrid.YSize; y++)
		{
			for (int x = 0; x < myGrid.XSize; x++)
			{
				int tileType = myGrid.Get(x, y);
				if (tileType > 0)
				{
					sprite.setPosition(x * 16, y * 16);

					switch (tileType)
					{
					case TILE_FILLED: spritePosRect.left = filled.x; spritePosRect.top = filled.y; break;
					case WALL_LEFT: spritePosRect.left = wallLeft.x; spritePosRect.top = wallLeft.y; break;
					case WALL_RIGHT: spritePosRect.left = wallRight.x; spritePosRect.top = wallRight.y; break;
					case WALL_DOWN: spritePosRect.left = wallDown.x; spritePosRect.top = wallDown.y; break;
					case WALL_UP: spritePosRect.left = wallUp.x; spritePosRect.top = wallUp.y; break;
					case WALL_CORNER_NE: spritePosRect.left = cornerNE.x; spritePosRect.top = cornerNE.y; break;
					case WALL_CORNER_NW: spritePosRect.left = cornerNW.x; spritePosRect.top = cornerNW.y; break;
					case WALL_CORNER_SE: spritePosRect.left = cornerSE.x; spritePosRect.top = cornerSE.y; break;
					case WALL_CORNER_SW: spritePosRect.left = cornerSW.x; spritePosRect.top = cornerSW.y; break;
					case DOOR_LEFT: spritePosRect.left = doorLeft.x; spritePosRect.top = doorLeft.y; break;
					case DOOR_RIGHT: spritePosRect.left = doorRight.x; spritePosRect.top = doorRight.y; break;
					case DOOR_DOWN: spritePosRect.left = doorDown.x; spritePosRect.top = doorDown.y; break;
					case DOOR_UP: spritePosRect.left = doorUp.x; spritePosRect.top = doorUp.y; break;
					case DOOR_NE_UP: spritePosRect.left = doorNEUp.x; spritePosRect.top = doorNEUp.y; break;
					case DOOR_NE_RIGHT: spritePosRect.left = doorNERight.x; spritePosRect.top = doorNERight.y; break;
					case DOOR_SE_DOWN: spritePosRect.left = doorSEDown.x; spritePosRect.top = doorSEDown.y; break;
					case DOOR_SE_RIGHT: spritePosRect.left = doorSERight.x; spritePosRect.top = doorSERight.y; break;
					case DOOR_NW_LEFT: spritePosRect.left = doorNWLeft.x; spritePosRect.top = doorNWLeft.y; break;
					case DOOR_NW_UP: spritePosRect.left = doorNWUp.x; spritePosRect.top = doorNWUp.y; break;
					case DOOR_SW_DOWN: spritePosRect.left = doorSWDown.x; spritePosRect.top = doorSWDown.y; break;
					case DOOR_SW_LEFT: spritePosRect.left = doorSWLeft.x; spritePosRect.top = doorSWLeft.y; break;
					default: assert(false);
					}

					sprite.setTextureRect(spritePosRect);
					window.draw(sprite);
				}
			}
		}
	}

private:
	sf::IntRect spritePosRect;
	sf::Texture texture;
	sf::Sprite sprite;
	Grid myGrid;

	sf::Vector2i wallDown = sf::Vector2i(0, 0);
	sf::Vector2i wallUp = sf::Vector2i(16, 0);
	sf::Vector2i wallRight = sf::Vector2i(32, 0);
	sf::Vector2i wallLeft= sf::Vector2i(48, 0);
	sf::Vector2i cornerSE = sf::Vector2i(0, 16);
	sf::Vector2i cornerNE = sf::Vector2i(16, 16);
	sf::Vector2i cornerNW = sf::Vector2i(32, 16);
	sf::Vector2i cornerSW = sf::Vector2i(48, 16);
	sf::Vector2i doorDown = sf::Vector2i(0, 32);
	sf::Vector2i doorUp = sf::Vector2i(16, 32);
	sf::Vector2i doorRight = sf::Vector2i(32, 32);
	sf::Vector2i doorLeft = sf::Vector2i(48, 32);
	sf::Vector2i doorSEDown = sf::Vector2i(0, 48);
	sf::Vector2i doorNEUp = sf::Vector2i(16, 48);
	sf::Vector2i doorNWLeft = sf::Vector2i(32, 48);
	sf::Vector2i doorSWLeft = sf::Vector2i(48, 48);
	sf::Vector2i doorSERight = sf::Vector2i(0, 64);
	sf::Vector2i doorNERight = sf::Vector2i(16, 64);
	sf::Vector2i doorNWUp = sf::Vector2i(32, 64);
	sf::Vector2i doorSWDown = sf::Vector2i(48, 64);
	sf::Vector2i filled = sf::Vector2i(0, 80);
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