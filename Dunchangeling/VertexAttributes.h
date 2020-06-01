#pragma once

struct VertexAttributes
{
	VertexAttributes(bool entry, bool endRoom) : isEntry(entry), isEndRoom(endRoom) {};
	VertexAttributes() {};

	std::vector<int> Objects;

	bool isEntry = false;
	bool isEndRoom = false;
};