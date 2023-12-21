#include <stdbool.h>
#include "raylib.h"
#include "structs.h"
#include "raymath.h"
#include "world.h"



RayCastResult RayCast(Vector3 pos, Vector3 dir, float length){
    dir = Vector3Normalize(dir);

    RayCastResult raycastResult = {};
    raycastResult.distance = 0.0f;

    IntVector3 start = (IntVector3){(int)pos.x, (int)pos.y, (int)pos.z};
    raycastResult.previousBlock = (IntVector3){0,0,0};

    IntVector3 step = (IntVector3){dir.x > 0 ? 1 : -1,
                                   dir.y > 0 ? 1 : -1,
                                   dir.z > 0 ? 1 : -1};


    Vector3  delta = (Vector3){fabsf(1 / dir.x),
                            fabsf(1 / dir.y),
                            fabsf(1 / dir.z)};

    Vector3 dist = (Vector3){
            step.x > 0 ? (float)start.x + 1 - pos.x : pos.x - (float)start.x,
            step.y > 0 ? (float)start.y + 1 - pos.y : pos.y - (float)start.y,
            step.z > 0 ? (float)start.z + 1 - pos.z : pos.z - (float)start.z};

    Vector3 tMax = (Vector3){
                delta.x * dist.x,
                delta.y * dist.y,
                delta.z * dist.z};

    while (raycastResult.distance <= length)
    {
        Block block;
        if(GetBlockAtPos(start, &block)){
            if(block.block_id != 0){
                raycastResult.hitBlock = start;
                raycastResult.hasHit = true;
                return raycastResult;
            }
        }

        raycastResult.previousBlock = start;

        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z)
            {
                start.x += step.x;
                raycastResult.distance = tMax.x;
                tMax.x += delta.x;
            }
            else
            {
                start.z += step.z;
                raycastResult.distance = tMax.z;
                tMax.z += delta.z;
            }
        }
        else
        {
            if (tMax.y < tMax.z)
            {
                start.y += step.y;
                raycastResult.distance = tMax.y;
                tMax.y += delta.y;
            }
            else
            {
                start.z += step.z;
                raycastResult.distance = tMax.z;
                tMax.z += delta.z;
            }
        }
    }

    raycastResult.hasHit = false;
    return raycastResult;
}
