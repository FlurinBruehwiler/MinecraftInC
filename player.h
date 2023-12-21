#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

extern bool useDebugCam;
extern Camera3D debugCam;
extern Camera3D playerCam;
void InitPlayer();
void UpdatePlayer();
void DrawPlayer();

#endif
