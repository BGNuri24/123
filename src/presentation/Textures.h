#pragma once
#include "raylib.h"

// All textures used by the 3D view.
struct Textures {
    Texture2D wall;
    Texture2D floor;
};

Textures createTextures();              // builds the textures (call after InitWindow)
void destroyTextures(Textures& t);      // frees them (call before CloseWindow)