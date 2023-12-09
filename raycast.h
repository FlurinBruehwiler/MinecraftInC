#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdbool.h>
#include "raylib.h"
#include "structs.h"

bool Raycast(Vector3 pos, Vector3 dir, float length, IntVector3 * hitBlock, IntVector3* previousBlock, float* distance);

#endif
