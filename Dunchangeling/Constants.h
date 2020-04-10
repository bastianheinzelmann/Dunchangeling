#pragma once

#define DLLExport __declspec(dllexport)

typedef unsigned int PopId;
typedef unsigned int VertexName;

const int GENERATION_BITS = 10;
const int ID_BITS = 32 - GENERATION_BITS;