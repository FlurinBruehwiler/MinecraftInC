#ifndef TEXTURES_H
#define TEXTURES_H

#include "structs.h"
#include "raylib.h"

extern BlockTexture* joaChocoTexture;
extern BlockTexture* joaMiniTexture;
extern BlockTexture* joaSchoolTexture;
extern BlockTexture* joaTruckTexture;

extern int textureWidth;

extern Texture textureAtlas;

void LoadTextureAtlas();

#endif
