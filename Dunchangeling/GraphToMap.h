#pragma once

#include "Room.h"
#include "Constants.h"
#include <vector>
#include "Layout.h"
#include "GraphUtils.h"
#include "DungeonGrid.h"
#include "IntersectionResult.h"

namespace GraphToMap
{
	struct RoomCollection
	{
		DLLExport RoomCollection(std::vector<Room> rooms);
		DLLExport RoomCollection() {};

		std::vector<Room> Rooms;

		Room& operator[](std::size_t idx) { return Rooms[idx]; }
		const Room& operator[](std::size_t idx) const { return Rooms[idx]; }
	};

	class MapGenerator
	{
	public:
		DLLExport MapGenerator(RoomCollection roomCollection, Chains chains, BoostGraph graph);

		DLLExport Layout GenerateLayout(BoostGraph & graph, std::vector<Layout> & layoutHistory);

		Layout GetInitialLayout(Layout& layout, Chain chain, BoostGraph& graph, bool & success);
		bool PlaceRoom(Layout& layout, LayoutRoom newRoom);
		DLLExport std::vector<Layout> AddChain(Layout& layout, Chain chain, BoostGraph& graph, int cycles, int trials, int maxLayouts, float startTemperature, float endTemperature, std::vector<std::pair<Layout, std::string>> & debugLayouts);

		Layout PerturbLayout(Layout& layout, Chain & chain, std::string & action);
		DLLExport void PerturbShape(Layout& layout, Chain & chain);
		DLLExport void PerturbPosition(Layout& layout, Chain & chain, std::string & action);

		DLLExport bool IsLayoutValid(Layout& layout);
		bool IsDifferent(Layout& newLayout, std::vector<Layout>& otherLayouts, Chain& chain);
		bool IsDifferent(Layout& layout1, Layout& layout2, Chain& chain);

		std::vector<Room> GetValidRooms(int vertexIndex, Layout& layout);

		float ShapePerturbChance = 0.4f;
		float PositionPerturbChance = 0.8f;

		float allowedRoomOverlapping = 0.0f;
		float DifferenceScale = 0.4f;

		RoomCollection Rooms;
		BoostGraph Graph;
	};

	std::vector<IntersectionResult> getIntersections(std::vector<LayoutRoom>& adjacentRooms, LayoutRoom& room);
	void calculateConfigSpaces(RoomCollection& roomCollection);
	void GetAdjacentRooms(LayoutRoom& layoutRoom, Layout& layout, std::vector<LayoutRoom>& adjacentRooms);
	void GetNonAdjacentRooms(LayoutRoom& layoutRoom, Layout& layout, std::vector<LayoutRoom>& nonAdjacentRooms);
	bool CheckLayoutIntegrity(Layout & layout);
	DLLExport DungeonGrid LayoutToSingleGrid(Layout & layout);
}