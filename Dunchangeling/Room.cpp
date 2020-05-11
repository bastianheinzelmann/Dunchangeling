#include "Room.h"
#include <math.h>

Room::Room(Grid grid)
{
	this->RoomGrid = grid;
	
}

Grid Room::CalculateConfigGrid(Room & otherRoom)
{
	Grid configSpaceGrid(this->RoomGrid.XSize + (2 * otherRoom.RoomGrid.XSize), this->RoomGrid.YSize + (2*otherRoom.RoomGrid.YSize));
	int offsetX = otherRoom.RoomGrid.XSize;
	int offsetY = otherRoom.RoomGrid.YSize;

	configSpaceGrid.PivotX = offsetX;
	configSpaceGrid.PivotY = offsetY;

	// pivot point is always [0, 0]

	// initialize with this room
	for (int y = 0; y < this->RoomGrid.YSize; y++)
	{
		for (int x = 0; x < this->RoomGrid.XSize; x++)
		{
			configSpaceGrid.Set(x + offsetX, y + offsetY, RoomGrid.Get(x, y));
		}
	}

	// now the real work begins, we subtract the offset to keep our moving room in bounds
	// we are moving the pivot in the first two loops
	for (int configSpaceY = 0; configSpaceY < configSpaceGrid.YSize - offsetY + 1; configSpaceY++)
	{
		for (int configSpaceX = 0; configSpaceX < configSpaceGrid.XSize - offsetX + 1; configSpaceX++)
		{
			// now iterate through the moving rooms tiles
			bool isConfigSpace = false;
			bool isIntersecting = false;
			for (int movingY = 0; movingY < otherRoom.RoomGrid.YSize; movingY++)
			{
				for (int movingX = 0; movingX < otherRoom.RoomGrid.XSize; movingX++)
				{
					if (otherRoom.RoomGrid.Get(movingX, movingY) > 0)
					{
						int localGridVal = configSpaceGrid.Get(movingX + configSpaceX, movingY + configSpaceY);
						int currentX = movingX + configSpaceX;
						int currentY = movingY + configSpaceY;
						// its intersecting we have to break
						if (localGridVal > 0 && localGridVal != GRID_CONFIG_SPACE)
						{
							isIntersecting = true;
							break;
						}

						// we are currently not intersecting and looking for neighbouring tiles adjacent to this tile

						if (currentX - 1 >= 0)
						{
							int val = configSpaceGrid.Get(currentX - 1, currentY);
							if (val > 0 && val != GRID_CONFIG_SPACE)
							{
								isConfigSpace = true;
							}
						}

						if (currentX + 1 < configSpaceGrid.XSize)
						{
							int val = configSpaceGrid.Get(currentX  + 1, currentY);
							if (val > 0 && val != GRID_CONFIG_SPACE)
							{
								isConfigSpace = true;
							}
						}

						if (currentY - 1 >= 0)
						{
							int val = configSpaceGrid.Get(currentX, currentY - 1);
							if (val > 0 && val != GRID_CONFIG_SPACE)
							{
								isConfigSpace = true;
							}
						}

						if (currentY + 1 < configSpaceGrid.YSize)
						{
							int val = configSpaceGrid.Get(currentX, currentY + 1);
							if (val > 0 && val != GRID_CONFIG_SPACE)
							{
								isConfigSpace = true;
							}
						}
						
					}

				}
				if (isIntersecting) break;
			}

			if (isConfigSpace && !isIntersecting)
			{
				configSpaceGrid.Set(configSpaceX, configSpaceY, GRID_CONFIG_SPACE | configSpaceGrid.Get(configSpaceX, configSpaceY));
			}
		}
	}

	//this->ConfigGrids.push_back(configSpaceGrid);

	return configSpaceGrid;
}

int Room::GetRoomArea()
{
	if (RoomArea != -1)
	{
		return RoomArea;
	}
	else
	{
		int area = 0;

		for (int y = 0; y < RoomGrid.YSize; y++)
		{
			for (int x = 0; x < RoomGrid.XSize; x++)
			{
				if (RoomGrid.Get(x, y) >= GRID_FILLED_NORMAL)
				{
					++area;
				}
			}

		}

		RoomArea = area;
		return RoomArea;
	}
}