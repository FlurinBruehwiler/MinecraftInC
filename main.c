#include "raylib.h"
#include "textures.h"
#include "blocks.h"
#include "mesh.h"
#include "skybox.h"
#include "world.h"
#include "player.h"
#include "raymath.h"
#include "perlin.h"
#include <stdio.h>

int screenWidth = 1500;
int screenHeight = 1000;

void UpdateDrawFrame();

int main()
{
    InitWindow(screenWidth, screenHeight, "Minecraft in C");

    InitializePerlin();
    InitializeSkybox();

    LoadTextureAtlas();

    initialize_blocks();

    SetTargetFPS(200);
    DisableCursor();

    InitWorld();

    InitPlayer();

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

void PrintVector3(Vector3 vector3, int index){

    static char result[50];  // Assuming a reasonable maximum length for the string

    sprintf(result, "%.2f, %.2f, %.2f", vector3.x, vector3.y, vector3.z);

    DrawText(result, 10, (index + 1) * 50, 50, BLACK);
}

void UpdateDrawFrame()
{
    BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(useDebugCam ? debugCam : playerCam);
            DrawSkybox();

            GenChunksInRenderDistance();
            RegenDirtyChunks();
            UpdatePlayer();

            for (int i = 0; i < chunkCount; ++i) {
                if(chunks[i]){
                    DrawModel(chunks[i]->model, (Vector3) { 0, 0, 0 }, 1, WHITE);
                }
            }

            DrawPlayer();

        EndMode3D();

        PrintVector3(playerCam.position, 0);

        DrawFPS(50, 50);

    EndDrawing();
}
