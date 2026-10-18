#pragma once
#include "raylib.h"

void drawCentered(const char* text, int y, int size, Color color);
void drawOptions(const char* const* labels, int count, int selected, int startY);