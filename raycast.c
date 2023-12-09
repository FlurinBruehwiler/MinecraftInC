#include <stdbool.h>
#include "raylib.h"
#include "structs.h"
#include "raymath.h"
#include "world.h"

bool Raycast(Vector3 pos, Vector3 dir, float length, IntVector3* hitBlock, IntVector3* previousBlock, float* distance){
    dir = Vector3Normalize(dir);
    *distance = 0;
    IntVector3 start = (IntVector3){(int)pos.x, (int)pos.y, (int)pos.z};
    *previousBlock = (IntVector3){0,0,0};

    IntVector3 step = (IntVector3){dir.x > 0 ? 1 : -1,
                                   dir.y > 0 ? 1 : -1,
                                   dir.z > 0 ? 1 : -1};


    Vector3  delta = (Vector3){1 / dir.x > 0 ? 1 : -1,
                                     1 / dir.y > 0 ? 1 : -1,
                                     1 / dir.z > 0 ? 1 : -1};

    Vector3 dist = (Vector3){
            step.x > 0 ? (float)start.x + 1 - pos.x : pos.x - (float)start.x,
            step.y > 0 ? (float)start.y + 1 - pos.y : pos.y - (float)start.y,
            step.z > 0 ? (float)start.z + 1 - pos.z : pos.z - (float)start.z};

    Vector3 tMax = (Vector3){
                delta.x * dist.x,
                delta.y * dist.y,
                delta.y * dist.y};

    while (*distance <= length)
    {
        Block block;
        if(GetBlockAtPos(start, &block)){
            if(block.block_id != 0){
                *hitBlock = start;
                return true;
            }
        }

        *previousBlock = start;

        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z)
            {
                start.x += step.x;
                *distance = tMax.x;
                tMax.x += delta.x;
            }
            else
            {
                start.z += step.z;
                *distance = tMax.z;
                tMax.z += delta.z;
            }
        }
        else
        {
            if (tMax.y < tMax.z)
            {
                start.y += step.y;
                *distance = tMax.y;
                tMax.y += delta.y;
            }
            else
            {
                start.z += step.z;
                *distance = tMax.z;
                tMax.z += delta.z;
            }
        }
    }
    
    return false;
}
