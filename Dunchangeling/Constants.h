#pragma once

#define DLLExport __declspec(dllexport)

typedef unsigned int PopId;
typedef unsigned int VertexName;

const int GENERATION_BITS = 10;
const int ID_BITS = 32 - GENERATION_BITS;

// Grid constants

// Config space constants
const unsigned int GRID_EMTPY = 0;
const unsigned int GRID_FILLED_NORMAL = 1;
const unsigned int GRID_DOOR = 2;
const unsigned int GRID_FILLED_BOSS = 3;
const unsigned int GRID_FILLED_START = 4;
const unsigned int GRID_CONFIG_SPACE = 5;
const unsigned int GRID_HULL = 6;