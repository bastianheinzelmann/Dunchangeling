#pragma once

#include "Vector2.h"
#include "Constants.h"
#include <assert.h>

enum DoorOrientation {DOE_Up = DOOR_ORI_UP, DOE_Down = DOOR_ORI_DOWN, DOE_Left = DOOR_ORI_LEFT, DOE_Right = DOOR_ORI_RIGHT};

struct Door
{
	Door() {};
	Door(Vector2 pos1, Vector2 pos2)
	{
		Pos1 = pos1;
		Pos2 = pos2;

		if ((Pos1.X - Pos2.X) == -1)
		{
			Pos1Orientation = DoorOrientation::DOE_Right;
			Pos2Orientation = DoorOrientation::DOE_Left;
		}
		else if ((Pos1.X - Pos2.X) == 1)
		{
			Pos1Orientation = DoorOrientation::DOE_Left;
			Pos2Orientation = DoorOrientation::DOE_Right;
		}
		else if ((Pos1.Y - Pos2.Y) == -1)
		{
			Pos1Orientation = DoorOrientation::DOE_Down;
			Pos2Orientation = DoorOrientation::DOE_Up;
		}
		else if ((Pos1.Y - Pos2.Y) == 1)
		{
			Pos1Orientation = DoorOrientation::DOE_Up;
			Pos2Orientation = DoorOrientation::DOE_Down;
		}
		else
		{
			assert(false, "Cells are not adjacent");
		}
	}

	Vector2 Pos1;
	Vector2 Pos2;

	DoorOrientation Pos1Orientation;
	DoorOrientation Pos2Orientation;
};