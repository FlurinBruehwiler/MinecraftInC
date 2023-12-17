#ifndef TEXTURES_H
#define TEXTURES_H

#include "structs.h"
#include "raylib.h"

extern BlockTexture* dirtTexture;
extern BlockTexture* grassTexture;
extern BlockTexture* grassTopTexture;

extern int textureWidth;
extern int textureHeight;

extern Texture textureAtlas;

void LoadTextureAtlas();

#endif
