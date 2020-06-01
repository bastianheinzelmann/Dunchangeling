#pragma once

#include <string>

#define DLLExport __declspec(dllexport)

typedef unsigned int PopId;
typedef unsigned int VertexName;

const int GENERATION_BITS = 10;
const int ID_BITS = 32 - GENERATION_BITS;

// Grid constants

// Config space constants
const unsigned int GRID_EMTPY = 0;
const unsigned int GRID_FILLED_NORMAL = 1;
const unsigned int GRID_FILLED_BOSS = 2;
const unsigned int GRID_FILLED_START = 4;
const unsigned int GRID_CONFIG_SPACE = 8;
const unsigned int GRID_DOOR = 16;

const unsigned int TILE_FILLED = 1;
const unsigned int TILE_END_ROOM = 2;
const unsigned int TILE_START_ROOM = 3;

const unsigned int OBJECT_TREASURE = 1;
const unsigned int OBJECT_MONSTER_A = 2;

const int NUMCOLORS = 10;
const std::string NODECOLORLIST[]{ "#0000FF", "#008000", "#00FF00", "#800000", "#800080", "#0000FF", "#00FFFF", "#808000", "#CD5C5C", "#1ABC9C" };