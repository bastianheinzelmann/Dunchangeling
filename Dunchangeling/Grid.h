#pragma once

#include "Constants.h"
#include <iostream>

class Grid
{
public:
	Grid() { gridArray = nullptr; }
	DLLExport Grid(unsigned int xSize, unsigned int ySize, int pivotX = 0, int pivotY = 0);
	DLLExport Grid(unsigned int xSize, unsigned int ySize, unsigned int grid[], int pivotX = 0, int pivotY = 0);
	DLLExport ~Grid();

	DLLExport unsigned int Get(unsigned int x, unsigned int y) const;
	DLLExport void Set(unsigned int x, unsigned int y, unsigned int val);

	unsigned int XSize;
	unsigned int YSize;

	int PivotX;
	int PivotY;

	unsigned int* gridArray;
private:
};

DLLExport std::ostream& operator<<(std::ostream& out, const Grid& grid);