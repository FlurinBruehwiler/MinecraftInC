
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "raycast.h"
#include "world.h"

Camera3D camera_3d;

void PrintVector3(Vector3 vector3){
    printf("% .2f, ", vector3.x);
    printf("% .2f, ", vector3.y);
    printf("% .2f\n", vector3.z);
}

void InitPlayer(){
    camera_3d = (Camera3D){ };
    camera_3d.position = (Vector3) { 2, 2, 2};
    camera_3d.target = (Vector3) {0,0,0};
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
            Block block;
            if(GetBlockAtPos(blockPos, &block)){
                Chunk* chunkPtr = NULL;
                Chunk **chunk = &chunkPtr;
                if(GetChunkOfBlock(blockPos, chunk)){
                    block.block_id = 0;
                    (*chunk)->isDirty = true;
                    PrintVector3((Vector3){(float)blockPos.x, (float)blockPos.y, (float)blockPos.z});
                }
            }
        }
    }
}
