#include "GraphToMap.h"
#include "LayoutRoom.h"
#include "GeneticAlgorithmUtils.h"
#include <list>

GraphToMap::RoomCollection::RoomCollection(std::vector<Room> rooms)
{
	this->Rooms = rooms;
	for (int i = 0; i < rooms.size(); i++)
	{
		this->Rooms[i].RoomID = i;
		this->Rooms[i].ConfigGrids = std::vector<Grid>(rooms.size());
	}
	calculateConfigSpaces(*this);
}

GraphToMap::MapGenerator::MapGenerator(RoomCollection roomCollection, Chains chains, BoostGraph graph)
{
	Layout layout(boost::num_vertices(graph));
	this->Graph = graph;
	this->Rooms = roomCollection;

	GetInitialLayout(layout, chains[0], graph);
}

// should make sure all ConfigGrids in rooms are empty before so that ids are correct
void GraphToMap::calculateConfigSpaces(RoomCollection& roomCollection)
{
	for (int i = 0; i < roomCollection.Rooms.size(); i++)
	{
		Room& currentRoom = roomCollection[i];
		for (int j = 0; j < roomCollection.Rooms.size(); j++)
		{
			Room& otherRoom = roomCollection[j];
			Grid configGrid = currentRoom.CalculateConfigGrid(otherRoom);
			currentRoom.ConfigGrids[j] = configGrid;
		}
	}
}

Layout GraphToMap::MapGenerator::GetInitialLayout(Layout & layout, Chain chain, BoostGraph& graph)
{
	std::list<int> queue;

	int firstIndex = -1;

	// first lay out all vertices that have already placed neighbours
	if (std::any_of(layout.LaidOutVertices.begin(), layout.LaidOutVertices.end(), [](bool v) { return v; }))
	{
		for (int i = 0; i < chain.size(); i++)
		{
			int chainVertexIndex = chain[i];

			auto neighbours = boost::adjacent_vertices(chainVertexIndex, graph);
			if (std::any_of(neighbours.first, neighbours.second, [layout](int v) { return layout.LaidOutVertices[v];}))
			{
				queue.push_back(chainVertexIndex);
			}
		}
	}
	else
	{
		queue.push_back(chain[0]);
		chain.erase(chain.begin());
	}

	assert(queue.size() > 0);

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		// create layout room
		LayoutRoom layoutRoom;
		layoutRoom.Room = this->Rooms[randomNumber(0, Rooms.Rooms.size() - 1)];
		layoutRoom.VertexID = u;

		// add adjacent rooms to room and add them to the queue on the way
		auto neighbours = boost::adjacent_vertices(u, graph);
		for (; neighbours.first != neighbours.second; neighbours.first++)
		{
			layoutRoom.Neighbours.push_back(*neighbours.first);
			for (int i = 0; i < chain.size(); i++)
			{
				if (chain[i] == *neighbours.first)
				{
					queue.push_back(chain[i]);
					chain.erase(chain.begin() + i);
				}
			}
		}
	
		PlaceRoom(layout, layoutRoom);
	}

	return layout;
}

void GraphToMap::MapGenerator::PlaceRoom(Layout & layout, LayoutRoom newRoom)
{
	assert(layout.Rooms[newRoom.VertexID].VertexID == -1);
	assert(!layout.LaidOutVertices[newRoom.VertexID]);

	if (std::any_of(layout.LaidOutVertices.begin(), layout.LaidOutVertices.end(), [](bool v) { return v; }))
	{
		// first get neighbours?
		// Check for intersection could be a interface function for room designs
		std::vector<LayoutRoom> adjacentRooms;
		for (int i = 0; i < newRoom.Neighbours.size(); i++)
		{
			int neighbouringIndex = newRoom.Neighbours[i];
			if (layout.LaidOutVertices[neighbouringIndex])
			{
				adjacentRooms.push_back(layout.Rooms[neighbouringIndex]);
			}
		}

		auto intersectionPositions = getIntersections(adjacentRooms, newRoom);
		// search for the position with best energy
		int bestEnergyIndex = -1;
		float lowestEnergy = INT_MAX;

		for (int i = 0; i < intersectionPositions.size(); i++)
		{
			newRoom.PosX = intersectionPositions[i].first;
			newRoom.PosY = intersectionPositions[i].second;
			layout.Rooms[newRoom.VertexID] = newRoom;

			float currentEnergy = layout.GetEnergy();

			if (currentEnergy < lowestEnergy)
			{
				bestEnergyIndex = i;
				lowestEnergy = currentEnergy;
			}
		}

		assert(bestEnergyIndex != -1);

		newRoom.PosX = intersectionPositions[bestEnergyIndex].first;
		newRoom.PosY = intersectionPositions[bestEnergyIndex].second;
		layout.Rooms[newRoom.VertexID] = newRoom;
		layout.LaidOutVertices[newRoom.VertexID] = true;
	}
	else
	{
		// layout is empty
		newRoom.PosX = 0;
		newRoom.PosY = 0;
		layout.Rooms[newRoom.VertexID] = newRoom;
		layout.LaidOutVertices[newRoom.VertexID] = true;
	}
}

std::vector<std::pair<int, int>> GraphToMap::getIntersections(std::vector<LayoutRoom>& adjacentRooms, LayoutRoom & room)
{
	std::vector<std::pair<int, int>> positions;

	int roomId = room.Room.RoomID;

	Grid firstConfigGrid = adjacentRooms[0].Room.ConfigGrids[roomId];
	int pivotWorldX = adjacentRooms[0].PosX - firstConfigGrid.PivotX;
	int pivotWorldY = adjacentRooms[0].PosY - firstConfigGrid.PivotY;

	// now the real interseciton check begins
	for (int y = 0; y < firstConfigGrid.YSize; y++)
	{
		for (int x = 0; x < firstConfigGrid.XSize; x++)
		{
			if ((GRID_CONFIG_SPACE & firstConfigGrid.Get(x, y)) == GRID_CONFIG_SPACE)
			{
				int worldX = x + pivotWorldX;
				int worldY = y + pivotWorldY;

				bool isIntersecting = true;

				for (int i = 1; i < adjacentRooms.size(); i++)
				{
					Grid& currentGrid = adjacentRooms[i].Room.ConfigGrids[roomId];

					int pX = adjacentRooms[i].PosX - currentGrid.PivotX;
					int pY = adjacentRooms[i].PosY - currentGrid.PivotY;
					int localX = worldX - pX;
					int localY = worldY - pY;
					if (localX < 0 || localY < 0)
					{
						isIntersecting = false;
					}
					else if (!((currentGrid.Get(localX, localY) & GRID_CONFIG_SPACE) == GRID_CONFIG_SPACE))
					{
						isIntersecting = false;
					}
				}

				if (isIntersecting)
				{
					positions.push_back(std::pair<int, int>(worldX, worldY));
				}
			}
		}
	}

	return positions;
}