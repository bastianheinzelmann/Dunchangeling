#pragma once

struct VertexAttributes
{
	VertexAttributes(bool entry, bool endRoom) : isEntry(entry), isEndRoom(endRoom) {};
	VertexAttributes() {};

	bool isEntry = false;
	bool isEndRoom = false;
};