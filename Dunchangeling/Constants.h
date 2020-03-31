#pragma once

#define DLLExport __declspec(dllexport)

typedef unsigned int PopId;

const int GENERATION_BITS = 10;
const int ID_BITS = 32 - GENERATION_BITS;