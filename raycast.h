#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdbool.h>
#include "raylib.h"
#include "structs.h"

RayCastResult RayCast(Vector3 pos, Vector3 dir, float length);

#endif
