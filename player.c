
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "raycast.h"
#include "world.h"

Camera3D camera_3d;

void InitPlayer(){
    camera_3d = (Camera3D){ };
    camera_3d.position = (Vector3) { -5, 40, -5};
    camera_3d.target = (Vector3) {40,0,40};
    camera_3d.up = (Vector3) {0, 1, 0};
    camera_3d.fovy = 100;
}

void UpdatePlayer()
{
    UpdateCamera(&camera_3d, CAMERA_CUSTOM);

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector3 dir = Vector3Subtract(camera_3d.target, camera_3d.position);
        IntVector3 blockPos;
        IntVector3 previousBlock;
        float distance;
        if(Raycast(camera_3d.position, dir, 10, &blockPos, &previousBlock, &distance)){
            Block* blockPtr = NULL;
            Block** block = &blockPtr;
            if(GetBlockPtrAtPos(blockPos, block)){
                Chunk* chunkPtr = NULL;
                Chunk **chunk = &chunkPtr;
                if(GetChunkOfBlock(blockPos, chunk)){
                    (*block)->block_id = 0;
                    (*chunk)->isDirty = true;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        Vector3 dir = Vector3Subtract(camera_3d.target, camera_3d.position);
        IntVector3 blockPos;
        IntVector3 previousBlock;
        float distance;
        if(Raycast(camera_3d.position, dir, 10, &blockPos, &previousBlock, &distance)){
            Block* blockPtr = NULL;
            Block** block = &blockPtr;
            if(GetBlockPtrAtPos(previousBlock, block)){
                Chunk* chunkPtr = NULL;
                Chunk **chunk = &chunkPtr;
                if(GetChunkOfBlock(previousBlock, chunk)){
                    (*block)->block_id = 1;
                    (*chunk)->isDirty = true;
                }
            }
        }
    }
}
