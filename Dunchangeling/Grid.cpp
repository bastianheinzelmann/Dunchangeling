#include "Grid.h"
#include <iostream>
#include <string>
#include <assert.h>

Grid::Grid(unsigned int xSize, unsigned int ySize, int pivotX, int pivotY)
{
	XSize = xSize;
	YSize = ySize;

	PivotX = pivotX;
	PivotY = pivotY;

	gridArray = new unsigned int[ySize * xSize];

	for (int i = 0; i < XSize * YSize; i++)
	{
		gridArray[i] = 0;
	}
}

Grid::Grid(unsigned int xSize, unsigned int ySize, unsigned int * grid, int pivotX, int pivotY)
{
	XSize = xSize;
	YSize = ySize;

	PivotX = pivotX;
	PivotY = pivotY;

	gridArray = grid;
}

Grid::~Grid()
{
}

unsigned int Grid::Get(unsigned int x, unsigned int y) const
{
	assert(x >= 0 && x < XSize && y >= 0 && y < YSize);
	return gridArray[XSize * y + x];
}

void Grid::Set(unsigned int x, unsigned int y, unsigned int val)
{
	assert(x >= 0 && x < XSize && y >= 0 && y < YSize);
	gridArray[XSize * y + x] = val;
}

std::ostream& operator<<(std::ostream& out, const Grid& grid)
{
	std::string gridString = "";

	for (int y = 0; y < grid.YSize; y++)
	{
		for (int x = 0; x < grid.XSize; x++)
		{
			gridString.append(std::to_string(grid.Get(x, y)));
		}
		gridString.append("\n");
	}
	out << gridString;
	return out;
}