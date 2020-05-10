#include "GraphToMap.h"
#include "LayoutRoom.h"
#include "GeneticAlgorithmUtils.h"
#include <list>

GraphToMap::RoomCollection::RoomCollection(std::vector<Room> rooms)
{
	this->Rooms = rooms;
	for (int i = 0; i < rooms.size(); i++)
	{
		rooms[i].RoomID = i;
	}
}

// should make sure all ConfigGrids in rooms are empty before so that ids are correct
void GraphToMap::calculateConfigSpaces(RoomCollection& roomCollection)
{
	for (int i = 0; i < roomCollection.Rooms.size(); i++)
	{
		Room& currentRoom = roomCollection[i];
		for (int j = 0; i < roomCollection.Rooms.size(); j++)
		{
			Room& otherRoom = roomCollection[i];
			Grid configGrid = currentRoom.CalculateConfigGrid(otherRoom);
			currentRoom.ConfigGrids.push_back(configGrid);
		}
	}
}

Layout GraphToMap::MapGenerator::GetInitialLayout(Layout & layout, Chain chain, BoostGraph& graph)
{
	std::list<int> queue;

	int firstIndex = -1;

	// first lay out all vertices that have already placed neighbours
	if (layout.Rooms.size() > 0)
	{
		for (int i = 0; i < chain.size(); i++)
		{
			int chainVertexIndex = chain[i];
			if (std::any_of(layout.LaidOutVertices.begin(), layout.LaidOutVertices.end(), [chainVertexIndex, graph] (int laidOutVert)
			{ 
				auto neighbours = boost::adjacent_vertices(laidOutVert, graph);
				return std::any_of(neighbours.first, neighbours.second, [chainVertexIndex](int neighbourVertex) { return neighbourVertex == chainVertexIndex; });
			}))
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
		layoutRoom.Room = this->Rooms[randomNumber(0, Rooms.Rooms.size())];
		layoutRoom.VertexID = u;
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



	}

	// add remaining vertexIndices with breadh first search


	return Layout();
}

void GraphToMap::MapGenerator::PlaceRoom(Layout & layout, LayoutRoom layoutRoom)
{

}