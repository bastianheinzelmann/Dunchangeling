#pragma once

#include "Vector2.h"
#include "Constants.h"
#include <assert.h>
#include "DungeonGrid.h"

enum DoorOrientation {DOE_Up = DOOR_ORI_UP, DOE_Down = DOOR_ORI_DOWN, DOE_Left = DOOR_ORI_LEFT, DOE_Right = DOOR_ORI_RIGHT};
//enum DoorOrientation {DOE_Up = 1, DOE_Down = 3, DOE_Left = 4, DOE_Right = 2};

struct Door
{
	Door() {};
	Door(Vector2 pos1, Vector2 pos2)
	{
		Pos1 = pos1;
		Pos2 = pos2;

		if ((Pos1.X - Pos2.X) == -1)
		{
			Pos1Orientation = DungeonData::DDE_East;
			Pos2Orientation = DungeonData::DDE_West;
		}
		else if ((Pos1.X - Pos2.X) == 1)
		{
			Pos1Orientation = DungeonData::DDE_West;
			Pos2Orientation = DungeonData::DDE_East;
		}
		else if ((Pos1.Y - Pos2.Y) == -1)
		{
			Pos1Orientation = DungeonData::DDE_South;
			Pos2Orientation = DungeonData::DDE_North;
		}
		else if ((Pos1.Y - Pos2.Y) == 1)
		{
			Pos1Orientation = DungeonData::DDE_North;
			Pos2Orientation = DungeonData::DDE_South;
		}
		else
		{
			assert(false, "Cells are not adjacent");
		}
	}

	Vector2 Pos1;
	Vector2 Pos2;

	DungeonData Pos1Orientation;
	DungeonData Pos2Orientation;
};