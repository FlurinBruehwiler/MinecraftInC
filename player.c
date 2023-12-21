
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "raycast.h"
#include "world.h"

Camera3D playerCam;
Camera3D debugCam;
bool useDebugCam = false;

void InitPlayer(){
    playerCam = (Camera3D){ };
    playerCam.position = (Vector3) {-5, 80, -5};
    playerCam.target = (Vector3) {40, 0, 40};
    playerCam.up = (Vector3) {0, 1, 0};
    playerCam.fovy = 100;

    debugCam = (Camera3D){ };
    debugCam.position = (Vector3) {-5, 80, -5};
    debugCam.target = (Vector3) {40, 0, 40};
    debugCam.up = (Vector3) {0, 1, 0};
    debugCam.fovy = 100;
}

float playerSpeed = 0.1f;

Vector3 GetMovementDelta(){
    Vector3 moveDelta = {};

    if (IsKeyDown(KEY_W))
    {
        moveDelta.x += playerSpeed;
    }

    if (IsKeyDown(KEY_S))
    {
        moveDelta.x -= playerSpeed;
    }

    if (IsKeyDown(KEY_D))
    {
        moveDelta.y += playerSpeed;
    }

    if (IsKeyDown(KEY_A))
    {
        moveDelta.y -= playerSpeed;
    }

    if (IsKeyDown(KEY_SPACE))
    {
        moveDelta.z += playerSpeed;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        moveDelta.z -= playerSpeed;
    }

    return moveDelta;
}

void HandleSpeedChange(){
    if (GetMouseWheelMoveV().y > 0)
    {
        playerSpeed *= 1.1f;
    }
    else if (GetMouseWheelMoveV().y < 0)
    {
        playerSpeed *= 0.9f;
    }

    if(playerSpeed < 0.01){
        playerSpeed = 0.01f;
    }
}

void ForwardCollisions(Vector3* velocity);

void UpdatePlayer()
{
    HandleSpeedChange();

    Camera3D* camera;

    if(useDebugCam){
        camera = &debugCam;
    }else{
        camera = &playerCam;
    }

    Vector3 moveDelta = GetMovementDelta();

    ForwardCollisions(&moveDelta);

    UpdateCameraPro(camera,
                    moveDelta,                                                // Move up-down,
                    (Vector3){
                            GetMouseDelta().x * 0.1f,                            // Rotation: yaw
                            GetMouseDelta().y * 0.1f,                            // Rotation: pitch
                            0.0f                                                // Rotation: roll
                    },
                    0);

    if(IsKeyPressed(KEY_LEFT_SHIFT)){
        useDebugCam = !useDebugCam;
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector3 dir = Vector3Subtract(playerCam.target, playerCam.position);
        RayCastResult hit = RayCast(playerCam.position, dir, 10);
        if(hit.hasHit){
            Block* blockPtr = NULL;
            Block** block = &blockPtr;
            if(GetBlockPtrAtPos(hit.hitBlock, block)){
                Chunk* chunkPtr = NULL;
                Chunk **chunk = &chunkPtr;
                if(GetChunkOfBlock(hit.hitBlock, chunk)){
                    (*block)->block_id = 0;
                    (*chunk)->isDirty = true;
                }
            }
        }
    }
    else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        Vector3 dir = Vector3Subtract(playerCam.target, playerCam.position);

        RayCastResult hit = RayCast(playerCam.position, dir, 10);

        if(hit.hasHit){
            Block* blockPtr = NULL;
            Block** block = &blockPtr;
            if(GetBlockPtrAtPos(hit.previousBlock, block)){
                Chunk* chunkPtr = NULL;
                Chunk **chunk = &chunkPtr;
                if(GetChunkOfBlock(hit.previousBlock, chunk)){
                    (*block)->block_id = 1;
                    (*chunk)->isDirty = true;
                }
            }
        }
    }
}

const float height = 1.8f;
const float width = 0.6f;
const float camera_height = 1.6f;

const float skinWidth = 0.015f;
const float reducedWidth = width - (skinWidth * 2);
const float reducedHeight = height - (skinWidth * 2);
const float halfReducedWidth = reducedWidth / 2;
const float halfReducedHeight = reducedHeight / 2;

float GetPlayerBottom(){
    return playerCam.position.y - camera_height;
}

Vector3 GetPlayerCenter(){
    return (Vector3){playerCam.position.x, GetPlayerBottom() + (height / 2), playerCam.position.z};
}

Vector2 forwardCollisions[] = {
        (Vector2){ +halfReducedWidth, -halfReducedHeight },
        (Vector2){ -halfReducedWidth, -halfReducedHeight },
        (Vector2){ +halfReducedWidth, +halfReducedHeight },
        (Vector2){ -halfReducedWidth, +halfReducedHeight },
};

float Sign(float f){
    if(f > 0){
        return 1;
    }
    if(f < 0){
        return -1;
    }
    return 0;
}

float Abs(float f){
    if(f < 0){
        return -f;
    }
    return f;
}

void ForwardCollisions(Vector3* velocity){
    Vector3 center = GetPlayerCenter();

    float direction = Sign(velocity->x);
    float rayLength = Abs(velocity->x) + skinWidth;

    for (int i = 0; i < 4; ++i) {
        Vector2 offset = forwardCollisions[i];

        float xOffset = (reducedWidth / 2) * (float)direction;
        Vector3 rayStart = (Vector3){ center.x + xOffset, center.y + offset.y, center.z + offset.x};

        Vector3 dirVector = (Vector3){(float)direction, 0, 0};
        RayCastResult hit = RayCast(rayStart, dirVector, rayLength);

        if(hit.hasHit){
            velocity->x = (hit.distance - skinWidth) * direction;
            rayLength = hit.distance;
        }

        printf("%.6f \n", rayLength);

        Vector3 rayEnd = rayStart;
        rayEnd.x += rayLength * (float)direction;
        DrawLine3D(rayStart, rayEnd, RED);
    }

    DrawCubeWires(GetPlayerCenter(), width, height, width, RED);
    DrawCube(playerCam.position, 0.1f, 0.1f, 0.1f, RED);
}

void DrawPlayer(){

}

