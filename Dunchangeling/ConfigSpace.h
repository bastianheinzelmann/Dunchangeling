#pragma once

#include "Grid.h"
#include "Constants.h"

class ConfigSpace
{
public:
	ConfigSpace(Grid grid) { ConfigSpaceGrid = grid; }

	Grid ConfigSpaceGrid;
};