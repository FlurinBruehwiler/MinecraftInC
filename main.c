#include "raylib.h"
#include "textures.h"
#include "blocks.h"
#include "mesh.h"
#include "skybox.h"
#include "world.h"
#include "player.h"
#include "raycast.h"
#include "raymath.h"
#include <stdio.h>

int screenWidth = 1500;
int screenHeight = 1000;

void UpdateDrawFrame(Model model);

int main()
{
    InitWindow(screenWidth, screenHeight, "Minecraft in C");

    //InitializeSkybox();

    LoadTextureAtlas();

    initialize_blocks();

    SetTargetFPS(200);
    DisableCursor();

    InitWorld();

    InitPlayer();

    while (!WindowShouldClose())
    {
        RegenDirtyChunks();
        UpdatePlayer();
        for (int x = 0; x < 10; ++x) {
            for (int y = 0; y < 10; ++y) {
                for (int z = 0; z < 10; ++z) {
                    if(chunks[x][y][z].hasMesh){
                        UpdateDrawFrame(chunks[x][y][z].model);
                    }
                }
            }
        }
    }

    CloseWindow();

    return 0;
}

void PrintVector3(Vector3 vector3, int index){

    static char result[50];  // Assuming a reasonable maximum length for the string

    sprintf(result, "%.2f, %.2f, %.2f", vector3.x, vector3.y, vector3.z);

    DrawText(result, 10, (index + 1) * 50, 50, BLACK);
}

Vector3 lastRayStart = {};
Vector3 lastRayEnd = {};

void UpdateDrawFrame(const Model model)
{
    BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera_3d);
            //DrawSkybox();
            DrawModel(model,  (Vector3) { 0,0,1 }, 1, WHITE);

            Vector3 dir = Vector3Subtract(camera_3d.target, camera_3d.position);
            DrawSphere(camera_3d.target, 0.5f, GREEN);
            DrawSphere(camera_3d.position, 0.5f, BLUE);
            IntVector3 block;
            IntVector3 previousBlock;
            float distance;

            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){

            }

            DrawLine3D((Vector3) { 0,0,0 }, dir, RED);
            DrawCube(dir, 1, 1, 1, BLUE);
            if(Raycast(camera_3d.position, dir, 100, &block, &previousBlock, &distance)){
                DrawCube((Vector3) { (float)block.x,(float)block.y, (float)block.z }, 1.1f, 1.1f, 1.1f, RED);
            }

        EndMode3D();

        PrintVector3(camera_3d.position, 0);
        PrintVector3(camera_3d.target, 1);
        PrintVector3(dir, 2);

        DrawFPS(50, 50);

    EndDrawing();
}
