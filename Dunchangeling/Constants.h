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

const unsigned int WALL_LEFT = 1;
const unsigned int WALL_RIGHT = 2;
const unsigned int WALL_UP = 4;
const unsigned int WALL_DOWN = 8;
const unsigned int WALL_CORNER_NE = 6;
const unsigned int WALL_CORNER_SE = 10;
const unsigned int WALL_CORNER_SW = 9;
const unsigned int WALL_CORNER_NW = 5;

const unsigned int DOOR_ORI_LEFT = 16;
const unsigned int DOOR_ORI_RIGHT = 32;
const unsigned int DOOR_ORI_UP = 64;
const unsigned int DOOR_ORI_DOWN = 128;

const unsigned int DOOR_LEFT = 17; // wall left + door ori left
const unsigned int DOOR_RIGHT = 34; // wall right + door ori right
const unsigned int DOOR_UP = 68; // you get the gist 
const unsigned int DOOR_DOWN = 136; // 
const unsigned int DOOR_NE_UP = 70;
const unsigned int DOOR_NE_RIGHT= 38;
const unsigned int DOOR_SE_DOWN = 138; // se = 10
const unsigned int DOOR_SE_RIGHT = 42;
const unsigned int DOOR_SW_DOWN = 137; // sw = 9
const unsigned int DOOR_SW_LEFT = 25;
const unsigned int DOOR_NW_LEFT = 21; //nw = 5
const unsigned int DOOR_NW_UP = 69;

const unsigned int TILE_FILLED = 100;

const int NUMCOLORS = 10;
const std::string NODECOLORLIST[]{ "#0000FF", "#008000", "#00FF00", "#800000", "#800080", "#0000FF", "#00FFFF", "#808000", "#CD5C5C", "#1ABC9C" };