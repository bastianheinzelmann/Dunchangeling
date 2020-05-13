#pragma once

#include "Room.h"
#include "Constants.h"
#include <vector>
#include "Layout.h"
#include "GraphUtils.h"

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
		Layout GetInitialLayout(Layout& layout, Chain chain, BoostGraph& graph);
		void PlaceRoom(Layout& layout, LayoutRoom newRoom);
		std::vector<Layout> AddChain(Layout& layout, Chain chain, BoostGraph& graph, int cycles, int trials, int maxLayouts, float startTemperature, float endTemperature);

		Layout PerturbLayout(Layout& layout, Chain & chain);
		void PerturbShape(Layout& layout, Chain & chain);
		void PerturbPosition(Layout& layout, Chain & chain);

		bool IsLayoutValid(Layout& layout);
		bool IsDifferent(Layout& newLayout, std::vector<Layout>& otherLayouts);

		float ShapePerturbChance = 0.4f;
		float PositionPerturChance = 0.8f;

		RoomCollection Rooms;
		BoostGraph Graph;
	};

	std::vector<std::pair<int, int>> getIntersections(std::vector<LayoutRoom>& adjacentRooms, LayoutRoom& room);
	void calculateConfigSpaces(RoomCollection& roomCollection);
}