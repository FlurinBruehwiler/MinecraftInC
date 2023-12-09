#include <stdlib.h>
#include "raylib.h"
#include "structs.h"
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

    InitializeSkybox();

    Texture textureAtlas = LoadTextureAtlas();

    initialize_blocks();

    SetTargetFPS(200);
    DisableCursor();

    InitWorld();

    Mesh *mesh = malloc(sizeof(Mesh));

    const int totalBlockCount = 32 * 32 * 32;

    VertexArray* vertexArray = create_vertex_array(48 * 3 * totalBlockCount);

    Chunk* chunk = &chunks[0][0][0];

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                Block block = chunk->blocks[x][y][z];

                if(block.block_id != 0){
                    AddBlock(LocalToGlobal(chunk, (IntVector3){x,y,z}), vertexArray, block.block_id);
                }
            }
        }
    }

    float* vertices = malloc(sizeof(float) * vertexArray->pos * 3);
    float* texCoords = malloc(sizeof(float) * vertexArray->pos * 2);

    for (int i = 0; i < vertexArray->pos - 1; ++i) {

        Vertex vertex = vertexArray->array[i];

        vertices[i * 3] = vertex.pos.x;
        vertices[i * 3 + 1] = vertex.pos.y;
        vertices[i * 3 + 2] = vertex.pos.z;

        texCoords[i * 2] = vertex.textCoord.x;
        texCoords[i * 2 + 1] = vertex.textCoord.y;
    }

    mesh->vertexCount = vertexArray->pos;
    mesh->triangleCount = vertexArray->pos;
    mesh->vertices = vertices;
    mesh->texcoords = texCoords;

    UploadMesh(mesh, false);
    const Model model = LoadModelFromMesh(*mesh);
    model.materials[0].maps->texture = textureAtlas;

    InitPlayer();

    while (!WindowShouldClose())
    {
        UpdatePlayer();
        UpdateDrawFrame(model);
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
