#include "Layout.h"
#include <unordered_set>
#include <math.h>
#include <boost/functional/hash.hpp>

Layout::Layout(int NumRooms, int nextChainIndex)
{
	Rooms = std::vector<LayoutRoom>(NumRooms);
	LaidOutVertices = std::vector<bool>(NumRooms, false);
	NextChainIndex = nextChainIndex;

	for (int i = 0; i < Rooms.size(); i++)
	{
		Rooms[i].VertexID = -1;
	}
}

float Layout::GetEnergy()
{
	float intersectionArea = GetIntersectionArea();
	float distances = GetDistances();
	float omega = GetAverageRoomArea();

	return std::exp(intersectionArea / omega) * std::exp(distances / omega) - 1;
}

bool Layout::isComplete()
{
	return std::all_of(LaidOutVertices.begin(), LaidOutVertices.end(), [](bool x) { return x; });
}

float Layout::GetAverageRoomArea()
{
	float placedRooms = 0;
	float areaSum = 0;

	for (auto i : Rooms)
	{
		if (i.VertexID != -1)
		{
			placedRooms += 1;
			areaSum += i.Room.GetRoomArea();
		}
	}

	return areaSum / placedRooms;
}

int Layout::GetDistances()
{
	std::vector<bool> distancesCalculated(this->Rooms.size(), false);
	int distanceSum = 0;

	for (int i = 0; i < Rooms.size(); i++)
	{
		if (LaidOutVertices[i])
		{
			for (auto j : Rooms[i].Neighbours)
			{
				if (!distancesCalculated[j] && Rooms[j].VertexID != -1)
				{
					LayoutRoom& otherRoom = Rooms[j];
					int distance = std::pow(otherRoom.PosX - Rooms[i].PosX, 2) + std::pow(otherRoom.PosY - Rooms[i].PosY, 2);
					distanceSum += distance;
				}
			}

			distancesCalculated[i] = true;
		}
	}

	return distanceSum;
}

int Layout::GetIntersectionArea()
{
	std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> visitedCoordinates;
	std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> intersections;

	for (auto i : this->Rooms)
	{
		if (i.VertexID != -1)
		{
			Grid& roomGrid = i.Room.RoomGrid;
			int pivotX = i.PosX - roomGrid.PivotX;
			int pivotY = i.PosY - roomGrid.PivotY;

			for (int y = 0; y < roomGrid.YSize; y++)
			{
				for (int x = 0; x < roomGrid.XSize; x++)
				{
					if (roomGrid.Get(x, y) >= GRID_FILLED_NORMAL)
					{
						int worldX = pivotX + x;
						int worldY = pivotY + y;

						std::pair<int, int> worldCoordinates(worldX, worldY);
						if (visitedCoordinates.count(worldCoordinates) > 0)
						{
							// intersection on this position
							intersections.insert(worldCoordinates);
						}
						else
						{
							// no intersection
							visitedCoordinates.insert(worldCoordinates);
						}
					}
				}

			}
		}
	}

	return intersections.size();
}