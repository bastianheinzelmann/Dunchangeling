
extern "C" __declspec(dllexport) struct  UDungeonProperties
{
	int NumSpecialRooms;
	int NumRooms;
	int NumOpponents;

	bool EndRoomDeadEnd;
	bool SpecialIsDeadEnd;
	bool FlankingRoutes;
	int NumFlankingRoutes;
	float branchingFactor;
};