#include "raylib.h"
#include "textures.h"
#include "blocks.h"
#include "mesh.h"
#include "skybox.h"
#include "world.h"
#include "player.h"

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

void UpdateDrawFrame(const Model model)
{
    BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera_3d);
            //DrawSkybox();
            DrawModel(model,  (Vector3) { 0,0,1 }, 1, WHITE);
        EndMode3D();

        DrawFPS(50, 50);

    EndDrawing();
}
