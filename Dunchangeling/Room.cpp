#include "Room.h"
#include <math.h>

Room::Room(Grid grid)
{
	this->RoomGrid = grid;
	this->RoomHull = CalculateHull();
	
}

Grid Room::CalculateHull()
{
	unsigned int sizeX = this->RoomGrid.XSize;
	unsigned int sizeY = this->RoomGrid.YSize;
	Grid grid(this->RoomGrid.XSize + 2, this->RoomGrid.YSize + 2);
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			int offsetX = x + 1;
			int offsetY = y + 1;

			if (RoomGrid.Get(x, y) > 0)
			{
				grid.Set(offsetX, offsetY, RoomGrid.Get(x, y));
				
				if (x == 0)
				{
					grid.Set(offsetX - 1, offsetY, GRID_HULL);
				}
				else if (this->RoomGrid.Get(x - 1, y) == 0)
				{
					grid.Set(offsetX - 1, offsetY, GRID_HULL);
				}
				
				if(x == sizeX - 1)
				{
					grid.Set(offsetX + 1, offsetY, GRID_HULL);
				}
				else if (this->RoomGrid.Get(x + 1, y) == 0)
				{
					grid.Set(offsetX + 1, offsetY, GRID_HULL);
				}
				
				if(y == 0)
				{
					grid.Set(offsetX, offsetY - 1, GRID_HULL);
				}
				else if (this->RoomGrid.Get(x, y - 1) == 0)
				{
					grid.Set(offsetX, offsetY - 1, GRID_HULL);
				}

				if (y == sizeY - 1)
				{
					grid.Set(offsetX, offsetY + 1, GRID_HULL);
				}
				else if (this->RoomGrid.Get(x, y + 1) == 0)
				{
					grid.Set(offsetX, offsetY + 1, GRID_HULL);
				}
			}
		}
	}

	return grid;
}

void Room::CalculateConfigGrid(Room & otherRoom)
{
	Grid configSpaceGrid(this->RoomGrid.XSize + (2 * otherRoom.RoomGrid.XSize), this->RoomGrid.YSize + (2*otherRoom.RoomGrid.YSize));
	

}