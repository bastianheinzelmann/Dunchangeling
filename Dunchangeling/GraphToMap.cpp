#include "GraphToMap.h"
#include "LayoutRoom.h"
#include "GeneticAlgorithmUtils.h"
#include <list>
#include <limits>
#include "Vector2.h"
#include "Door.h"
#include "DungeonGrid.h"

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
	Layout layout(boost::num_vertices(graph), 0);
	this->Graph = graph;
	this->Rooms = roomCollection;

	//GetInitialLayout(layout, chains[0], graph);
	//AddChain(layout, chains[0], graph, 5, 50, 4, 0.6f, 0.2f);
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

Layout GraphToMap::MapGenerator::GenerateLayout(BoostGraph & graph, std::vector<Layout> & layoutHistory)
{
	Chains chains = GeneticAlgorithmUtils::ChainDecomposition(graph);

	Layout startLayout = Layout(boost::num_vertices(graph), 0);
	std::list<Layout> stack;
	stack.push_back(startLayout);

	layoutHistory = std::vector<Layout>(chains.size());


	int chainIndex = 0;

	while (!stack.empty())
	{
		Layout layout = stack.back();
		stack.pop_back();

		// just for demonstration uses
		layoutHistory[layout.NextChainIndex] = layout;

		Chain chain = chains[layout.NextChainIndex];
		std::cout << "Chain index: " << layout.NextChainIndex << "\n";

		std::vector<std::pair<Layout, std::string>> debugLayout;

		std::vector<Layout> partialLayouts = AddChain(layout, chain, graph, 50, 100, 4, 0.6f, 0.2f, debugLayout);

		if (partialLayouts.size() > 0)
		{
			if (partialLayouts[0].IsComplete())
			{
				std::cout << "Layout generation successful.\n";
				return partialLayouts[0];
			}
			else
			{
				stack.insert(stack.end(), partialLayouts.begin(), partialLayouts.end());
				++chainIndex;
			}
		}
	}

	return Layout();
}

std::vector<Layout> GraphToMap::MapGenerator::AddChain(Layout & layout, Chain chain, BoostGraph & graph, int cycles, int trials, int maxLayouts, float startTemperature, float endTemperature, std::vector<std::pair<Layout, std::string>> & debugLayouts)
{
	assert(startTemperature > endTemperature);

	std::vector<Layout> generatedLayouts;
	bool initialLayoutSuccess;

	Layout currentLayout = GetInitialLayout(layout, chain, graph, initialLayoutSuccess);
	if (!initialLayoutSuccess)
	{
		return generatedLayouts;
	}
	else
	{
		++currentLayout.NextChainIndex;
	}

	debugLayouts.push_back(std::pair<Layout, std::string>(currentLayout, "InitialLayout"));

	float currentTemperature = startTemperature;
	float averageDeltaEnergy = 0;
	int acceptedLayouts = 0;

	for (int i = 0; i < cycles; i++)
	{
		if (generatedLayouts.size() >= maxLayouts)
		{
			break;
		}

		for (int j = 0; j < trials; j++)
		{
			std::string action = "";
			Layout newLayout = PerturbLayout(currentLayout, chain, action);

			if (IsLayoutValid(newLayout))
			{
				if (IsDifferent(newLayout, generatedLayouts, chain))
				{
					generatedLayouts.push_back(newLayout);
				}
			}

			float energyDelta = newLayout.GetEnergy() - currentLayout.GetEnergy();
			float energyDeltaAbs = std::abs(energyDelta);
			averageDeltaEnergy = ((acceptedLayouts * averageDeltaEnergy) + energyDeltaAbs) / (acceptedLayouts + 1);

			//std::cout << "Weird stuff: " << -energyDelta / (averageDeltaEnergy * currentTemperature) << "\n";

			if (energyDelta < 0)
			{
				currentLayout = newLayout;
				++acceptedLayouts;
				debugLayouts.push_back(std::pair<Layout, std::string>(currentLayout, action));
				//std::cout << "Current Energy: " << currentLayout.GetEnergy() << "\n";
			}
			else if (randomFloatNumber(0.0f, 1.0f) < std::exp(-energyDeltaAbs / (averageDeltaEnergy * currentTemperature)))
			{
				currentLayout = newLayout;
				++acceptedLayouts;
				debugLayouts.push_back(std::pair<Layout, std::string>(currentLayout, action));
				//std::cout << "Current Energy lucky: " << currentLayout.GetEnergy() << "\n";
			}
		}

		currentTemperature -= (startTemperature - endTemperature) / cycles;
	}

	return generatedLayouts;
}

bool GraphToMap::MapGenerator::IsDifferent(Layout & newLayout, std::vector<Layout>& otherLayouts, Chain& chain)
{
	for (auto i : otherLayouts)
	{
		if (!IsDifferent(newLayout, i, chain))
		{
			return false;
		}
	}

	return true;
}

bool GraphToMap::MapGenerator::IsDifferent(Layout & layout1, Layout & layout2, Chain& chain)
{
	int difference = 0;

	for (int i = 0; i < layout1.Rooms.size(); i++)
	{
		assert(layout1.LaidOutVertices[i] == layout2.LaidOutVertices[i]);
		
		if (layout1.LaidOutVertices[i])
		{
			difference += std::pow(layout1.Rooms[i].PosX - layout2.Rooms[i].PosX, 2) + std::pow(layout1.Rooms[i].PosY - layout2.Rooms[i].PosY, 2);
		}
	}

	return DifferenceScale * ((float)difference / chain.size()) >= 1;
}

Layout GraphToMap::MapGenerator::PerturbLayout(Layout & layout, Chain & chain, std::string & action)
{
	Layout newLayout = layout;

	if (randomFloatNumber(0.0f, 1.0f) <= ShapePerturbChance)
	{
		PerturbShape(newLayout, chain);
	}
	else
	{
		PerturbPosition(newLayout, chain, action);
	}

	return newLayout;
}

void GraphToMap::MapGenerator::PerturbShape(Layout & layout, Chain & chain)
{
	// we need to check if it can be replaced by a room regarding intersections and doors bros

	// get random element from chain
	int randChainIndex = randomNumber(0, chain.size() - 1);
	int randVertexIndex = chain[randChainIndex];

	std::vector<Room> validRooms = GetValidRooms(randVertexIndex, layout);

	if (validRooms.size() > 0)
	{
		//std::cout << "Actually found a roomshape that fits! Vertex: " << randVertexIndex << "\n";
		Room randRoom = validRooms[randomNumber(0, validRooms.size() - 1)];
		layout.Rooms[randVertexIndex].Room = randRoom;
	}
	else
	{
		//std::cout << "No suitable shape found \n";
	}

	assert(CheckLayoutIntegrity(layout));
}

void GraphToMap::MapGenerator::PerturbPosition(Layout & layout, Chain & chain, std::string & action)
{
	int randChainIndex = randomNumber(0, chain.size() - 1);
	int randVertexIndex = chain[randChainIndex];

	LayoutRoom& room = layout.Rooms[randVertexIndex];

	std::vector<LayoutRoom> adjacentRooms;

	assert(room.VertexID != -1);

	for (int i = 0; i < room.Neighbours.size(); i++)
	{
		int neighbouringIndex = room.Neighbours[i];
		if (layout.LaidOutVertices[neighbouringIndex])
		{
			adjacentRooms.push_back(layout.Rooms[neighbouringIndex]);
		}
	}

	auto intersectionPositions = getIntersections(adjacentRooms, room);

	int randomIntersection = -1;

	//assert(intersectionPositions.size() > 0);
	if (intersectionPositions.size() > 0)
	{
		randomIntersection = randomNumber(0, intersectionPositions.size() - 1);
		auto intersection = intersectionPositions[randomIntersection];
		room.PosX = intersection.first;
		room.PosY = intersection.second;
	}

	action.append("Vertex: ").append(std::to_string(randVertexIndex)).append(" Intersection Index ").append(std::to_string(randomIntersection));
	assert(CheckLayoutIntegrity(layout));
	//std::cout << "Perturbed position of vertex: " << randVertexIndex << " \n";
}

bool GraphToMap::MapGenerator::IsLayoutValid(Layout & layout)
{
	bool isValid = true;

	// go through every laid out room
	for (int i = 0; i < layout.Rooms.size(); i++)
	{
		if (layout.LaidOutVertices[i])
		{
			LayoutRoom& currentRoom = layout.Rooms[i];
			std::vector<LayoutRoom> nonAdjacentRooms;
			GetNonAdjacentRooms(currentRoom, layout, nonAdjacentRooms);

			int pivotX = currentRoom.PosX - currentRoom.Room.RoomGrid.PivotX;
			int pivotY = currentRoom.PosY - currentRoom.Room.RoomGrid.PivotY;

			// go through each adjacent room
			for (int j = 0; j < nonAdjacentRooms.size(); j++)
			{
				int intersectionAreaSize = 0;

				// check for area intersection
				for (int y = 0; y < currentRoom.Room.RoomGrid.YSize; y++)
				{
					for (int x = 0; x < currentRoom.Room.RoomGrid.XSize; x++)
					{
						if (currentRoom.Room.RoomGrid.Get(x, y) >= GRID_FILLED_NORMAL)
						{
							int worldX = pivotX + x;
							int worldY = pivotY + y;

							int pivotOtherX = nonAdjacentRooms[j].PosX - nonAdjacentRooms[j].Room.RoomGrid.PivotX;
							int pivotOtherY = nonAdjacentRooms[j].PosY - nonAdjacentRooms[j].Room.RoomGrid.PivotY;

							int localX = worldX - pivotOtherX;
							int localY = worldY - pivotOtherY;

							if (localX >= 0 && localX < nonAdjacentRooms[j].Room.RoomGrid.XSize && localY >= 0 && localY < nonAdjacentRooms[j].Room.RoomGrid.YSize)
							{
								if (nonAdjacentRooms[j].Room.RoomGrid.Get(localX, localY) >= GRID_FILLED_NORMAL)
								{
									++intersectionAreaSize;
								}
							}
						}
					}
				}

				float areaRatio = (float)intersectionAreaSize / currentRoom.Room.GetRoomArea();
				if (areaRatio > this->allowedRoomOverlapping)
				{
					return false;
				}
			}
		}
	}

	return isValid;
}

std::vector<Room> GraphToMap::MapGenerator::GetValidRooms(int vertexIndex, Layout & layout)
{
	std::vector<Room> validRooms;
	LayoutRoom& lroom = layout.Rooms[vertexIndex];
	int currentRoomID = lroom.Room.RoomID;
	std::vector<LayoutRoom> adjacentRooms;

	int worldX = lroom.PosX;
	int worldY = lroom.PosY;

	GetAdjacentRooms(lroom, layout, adjacentRooms);

	for (int i = 0; i < Rooms.Rooms.size(); i++)
	{
		bool isValid = false;
		Room currentRoom = Rooms[i];

		if (i != currentRoomID)
		{
			isValid = true;

			for (int j = 0; j < adjacentRooms.size(); j++)
			{
				Grid& configGrid = adjacentRooms[j].Room.ConfigGrids[currentRoom.RoomID];
				int worldPivotX = adjacentRooms[j].PosX - configGrid.PivotX;
				int worldPivotY = adjacentRooms[j].PosY - configGrid.PivotY;

				int localX = worldX - worldPivotX;
				int localY = worldY - worldPivotY;

				if (localX >= 0 && localX < configGrid.XSize && localY >= 0 && localY < configGrid.YSize)
				{
					if (!((configGrid.Get(localX, localY) & GRID_CONFIG_SPACE) == GRID_CONFIG_SPACE))
					{
						isValid = false;
						break;
					}
				}
				else
				{
					isValid = false;
					break;
				}
			}
		}

		if (isValid)
		{
			validRooms.push_back(currentRoom);
		}
	}

	return validRooms;
}

void GraphToMap::GetAdjacentRooms(LayoutRoom & layoutRoom, Layout & layout, std::vector<LayoutRoom>& adjacentRooms)
{
	for (int i = 0; i < layoutRoom.Neighbours.size(); i++)
	{
		int neighbouringIndex = layoutRoom.Neighbours[i];
		if (layout.LaidOutVertices[neighbouringIndex])
		{
			adjacentRooms.push_back(layout.Rooms[neighbouringIndex]);
		}
	}
}

void GraphToMap::GetNonAdjacentRooms(LayoutRoom & layoutRoom, Layout & layout, std::vector<LayoutRoom>& nonAdjacentRooms)
{
	for (int i = 0; i < layout.Rooms.size(); i++)
	{
		if (layout.LaidOutVertices[i] && i != layoutRoom.VertexID && std::find(layoutRoom.Neighbours.begin(), layoutRoom.Neighbours.end(), i) == layoutRoom.Neighbours.end())
		{
			nonAdjacentRooms.push_back(layout.Rooms[i]);
		}
	}
}

Layout GraphToMap::MapGenerator::GetInitialLayout(Layout & layout, Chain chain, BoostGraph& graph, bool & success)
{
	std::list<int> queue;

	int firstIndex = -1;
	success = true;

	// first lay out all vertices that have already placed neighbours
	if (std::any_of(layout.LaidOutVertices.begin(), layout.LaidOutVertices.end(), [](bool v) { return v; }))
	{
		for (int i = 0; i < chain.size(); i++)
		{
			int chainVertexIndex = chain[i];

			auto neighbours = boost::adjacent_vertices(chainVertexIndex, graph);
			if (std::any_of(neighbours.first, neighbours.second, [layout](int v) { return layout.LaidOutVertices[v]; }))
			{
				queue.push_back(chainVertexIndex);
				chain.erase(chain.begin() + i);
				break;
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
		layoutRoom.Attributes.isEndRoom = graph[u].isEndRoom;
		layoutRoom.Attributes.isEntry = graph[u].isEntry;
		layoutRoom.Attributes = graph[u];

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
	
		if (!PlaceRoom(layout, layoutRoom))
		{
			success = false;
			break;
		}
	}

	assert(chain.size() == 0, "Couldnt empty chain, failed..");
		
	return layout;
}

bool GraphToMap::MapGenerator::PlaceRoom(Layout & layout, LayoutRoom newRoom)
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
		float lowestEnergy = std::numeric_limits<float>::max();

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

		//assert(bestEnergyIndex != -1);
		if (bestEnergyIndex == -1)
		{
			return false;
		}

		newRoom.PosX = intersectionPositions[bestEnergyIndex].first;
		newRoom.PosY = intersectionPositions[bestEnergyIndex].second;
		layout.Rooms[newRoom.VertexID] = newRoom;
		layout.LaidOutVertices[newRoom.VertexID] = true;
	}
	else
	{
		// layout is empty
		for (int i = 0; i < layout.LaidOutVertices.size(); i++)
		{
			if (layout.LaidOutVertices[i])
			{
				assert(false);
			}
		}
		assert(!std::any_of(layout.LaidOutVertices.begin(), layout.LaidOutVertices.end(), [](int v) { return v; }));

		newRoom.PosX = 0;
		newRoom.PosY = 0;
		layout.Rooms[newRoom.VertexID] = newRoom;
		layout.LaidOutVertices[newRoom.VertexID] = true;
	}

	assert(CheckLayoutIntegrity(layout));

	return true;
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
					if (localX < 0 || localY < 0 || localX >= currentGrid.XSize || localY >= currentGrid.YSize)
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

DLLExport DungeonGrid GraphToMap::LayoutToSingleGrid(Layout & layout)
{
	int lowerXBound = INT_MAX, lowerYBound = INT_MAX, upperXBound = INT_MIN, upperYBound = INT_MIN;

	// first find boundaries
	for (auto i : layout.Rooms)
	{
		if (i.VertexID != -1)
		{
			if (i.PosX < lowerXBound)
			{
				lowerXBound = i.PosX;
			}
			if (i.PosY < lowerYBound)
			{
				lowerYBound = i.PosY;
			}
			int upperVal = i.PosX + i.Room.RoomGrid.XSize;
			if (upperVal > upperXBound)
			{
				upperXBound = upperVal;
			}
			upperVal = i.PosY + i.Room.RoomGrid.YSize;
			if (upperVal > upperYBound)
			{
				upperYBound = upperVal;
			}
		}
	}

	// create a new grid
	DungeonGrid dgrid(upperXBound - lowerXBound, upperYBound - lowerYBound);

	// now iterate over all rooms and create walls and doors
	std::vector<bool> doorsFound(layout.Rooms.size(), false);
	std::vector<Door> doorPositions;

	for (auto currentRoom : layout.Rooms)
	{
		if (currentRoom.VertexID != -1)
		{
			std::unordered_map<int, std::vector<Door>> possibleDoors;

			Grid & roomGrid = currentRoom.Room.RoomGrid;
			for (int y = 0; y < roomGrid.YSize; y++)
			{
				for (int x = 0; x < roomGrid.XSize; x++)
				{
					// world coordinates
					int worldPosX = x + currentRoom.PosX;
					int worldPosY = y + currentRoom.PosY;

					// makes only sense if there is a floor and the dungeongrid is empty on that position
					if (roomGrid.Get(x, y) >= GRID_FILLED_NORMAL && dgrid.Get(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile) < 1)
					{
						/*if (dgrid.Get(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile) > 0)
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 0);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 0);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 0);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 0);
						}*/

						unsigned int tileType;
						if (currentRoom.Attributes.isEndRoom)
						{
							tileType = TILE_END_ROOM;
						}
						else if (currentRoom.Attributes.isEntry)
						{
							tileType = TILE_START_ROOM;
						}
						else if (currentRoom.Attributes.treasureRoom)
						{
							tileType = TILE_SPECIAL_ROOM;
						}
						else if (roomGrid.Get(x, y) == GRID_FILLED_HAZARD)
						{
							tileType = TILE_HAZARD;
						}
						else
						{
							tileType = TILE_FILLED;
						}

						if (!currentRoom.Attributes.Opponents.empty())
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Object, currentRoom.Attributes.Opponents.front());
							currentRoom.Attributes.Opponents.pop_front();
						}

						unsigned int wallType = 0;

						if (x - 1 < 0 || roomGrid.Get(x - 1, y) == GRID_EMTPY)
						{
							// left is empty
							wallType += 1;
						}
						if (x + 1 >= roomGrid.XSize || roomGrid.Get(x + 1, y) == GRID_EMTPY)
						{
							// right is empty
							wallType += 2;
						}
						if (y - 1 < 0 || roomGrid.Get(x, y - 1) == GRID_EMTPY)
						{
							// up is empty
							wallType += 4;
						}
						if (y + 1 >= roomGrid.YSize || roomGrid.Get(x, y + 1) == GRID_EMTPY)
						{
							// down is empty
							wallType += 8;
						}

						// world coordinates
						int worldPosX = x + currentRoom.PosX;
						int worldPosY = y + currentRoom.PosY;

						switch (wallType)
						{
						case 0: 
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							break;
						}
						case 1:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						case 2:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							break;
						}
						case 3:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							break;
						}
						case 4:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							break;
						}
						case 5:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						case 6:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							break;
						}
						case 7:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						case 8:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							break;
						}
						case 9:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						case 10:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							break;
						}
						case 11:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							break;
						}
						case 12:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							break;
						}
						case 13:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						case 14:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							break;
						}
						case 15:
						{
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, tileType);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_South, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_North, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_East, 1);
							dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_West, 1);
							break;
						}
						}

						for (auto neighbour : currentRoom.Neighbours)
						{

							if (layout.Rooms[neighbour].VertexID != -1 && !doorsFound[neighbour])
							{
								LayoutRoom & neighbourRoom = layout.Rooms[neighbour];
								int localPosX = worldPosX - neighbourRoom.PosX;
								int localPosY = worldPosY - neighbourRoom.PosY;

								if (localPosX - 1 >= 0 && localPosY >= 0 
									&& localPosX - 1 < neighbourRoom.Room.RoomGrid.XSize && localPosY < neighbourRoom.Room.RoomGrid.YSize 
									&& neighbourRoom.Room.RoomGrid.Get(localPosX - 1, localPosY) >= GRID_FILLED_NORMAL && dgrid.Get(worldPosX - 1 - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile) == 0)
								{
									possibleDoors[neighbour].push_back(Door(Vector2(worldPosX, worldPosY), Vector2(worldPosX - 1, worldPosY)));
								}
								if (localPosX + 1 >= 0 && localPosY >= 0
									&& localPosX + 1 < neighbourRoom.Room.RoomGrid.XSize && localPosY < neighbourRoom.Room.RoomGrid.YSize
									&& neighbourRoom.Room.RoomGrid.Get(localPosX + 1, localPosY) >= GRID_FILLED_NORMAL && dgrid.Get(worldPosX + 1 - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile) == 0)
								{
									possibleDoors[neighbour].push_back(Door(Vector2(worldPosX, worldPosY), Vector2(worldPosX + 1, worldPosY)));
								}
								if (localPosX >= 0 && localPosY - 1 >= 0
									&& localPosX < neighbourRoom.Room.RoomGrid.XSize && localPosY - 1 < neighbourRoom.Room.RoomGrid.YSize
									&& neighbourRoom.Room.RoomGrid.Get(localPosX, localPosY - 1) >= GRID_FILLED_NORMAL && dgrid.Get(worldPosX - lowerXBound, worldPosY - 1 - lowerYBound, DungeonData::DDE_Tile) == 0)
								{
									possibleDoors[neighbour].push_back(Door(Vector2(worldPosX, worldPosY), Vector2(worldPosX , worldPosY - 1)));
								}
								if (localPosX >= 0 && localPosY + 1 >= 0
									&& localPosX < neighbourRoom.Room.RoomGrid.XSize && localPosY + 1 < neighbourRoom.Room.RoomGrid.YSize
									&& neighbourRoom.Room.RoomGrid.Get(localPosX, localPosY + 1) >= GRID_FILLED_NORMAL && dgrid.Get(worldPosX - lowerXBound, worldPosY + 1 - lowerYBound, DungeonData::DDE_Tile) == 0)
								{
									possibleDoors[neighbour].push_back(Door(Vector2(worldPosX, worldPosY), Vector2(worldPosX, worldPosY + 1)));
								}
							}
						}
					}
					else
					{
						// grid is empty
						//dgrid.Set(worldPosX - lowerXBound, worldPosY - lowerYBound, DungeonData::DDE_Tile, 0);
					}
				}
			}

			//assert(possibleDoors.size() > 0, "No doors for room found");

			for (auto doors : possibleDoors)
			{
				Door doorPos = doors.second[randomNumber(0, doors.second.size() - 1)];
				doorPositions.push_back(doorPos);
			}

			doorsFound[currentRoom.VertexID] = true;
		}
	}

	for (auto door : doorPositions)
	{
		assert(dgrid.Get(door.Pos1.X - lowerXBound, door.Pos1.Y - lowerYBound, DungeonData::DDE_Tile) != 0);
		assert(dgrid.Get(door.Pos2.X - lowerXBound, door.Pos2.Y - lowerYBound, DungeonData::DDE_Tile) != 0);

		dgrid.Set(door.Pos1.X - lowerXBound, door.Pos1.Y - lowerYBound, door.Pos1Orientation, 2);
		dgrid.Set(door.Pos2.X - lowerXBound, door.Pos2.Y - lowerYBound, door.Pos2Orientation, 2);
	}

	return dgrid;
}

bool GraphToMap::CheckLayoutIntegrity(Layout & layout)
{
	for (auto currentRoom : layout.Rooms)
	{
		if (currentRoom.VertexID != -1)
		{
			Grid& currentGrid = currentRoom.Room.RoomGrid;
			int pivotX = currentRoom.PosX - currentGrid.PivotX;
			int pivotY = currentRoom.PosY - currentGrid.PivotY;

			for (int y = 0; y < currentGrid.YSize; y++)
			{
				for (int x = 0; x < currentGrid.XSize; x++)
				{
					if (currentGrid.Get(x, y) >= GRID_FILLED_NORMAL)
					{
						int worldX = pivotX + x;
						int worldY = pivotY + y;

						for (auto n : currentRoom.Neighbours)
						{
							LayoutRoom& adjacentRoom = layout.Rooms[n];
							if (layout.Rooms[n].VertexID != -1)
							{
								int localX = worldX - adjacentRoom.PosX;
								int localY = worldY - adjacentRoom.PosY;

								if (localX >= 0 && localX < adjacentRoom.Room.RoomGrid.XSize && localY >= 0 && localY < adjacentRoom.Room.RoomGrid.YSize)
								{
									if (adjacentRoom.Room.RoomGrid.Get(localX, localY) >= GRID_FILLED_NORMAL)
									{
										return false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}