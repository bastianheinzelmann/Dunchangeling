#pragma once

struct VertexAttributes
{
	VertexAttributes(bool entry, bool endRoom) : isEntry(entry), isEndRoom(endRoom) {};
	VertexAttributes() {};

	bool treasureRoom = false;
	bool isEntry = false;
	bool isEndRoom = false;

	// key: id	value: number of opponent type
	std::list<int> Opponents;
};