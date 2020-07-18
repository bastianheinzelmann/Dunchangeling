#pragma once

struct OpponentInfo
{
	OpponentInfo() {}

	OpponentInfo(unsigned int id, unsigned int difficulty)
	{
		Id = id;
		Difficulty = difficulty;
	}

	unsigned int Id;
	unsigned int Difficulty;
};