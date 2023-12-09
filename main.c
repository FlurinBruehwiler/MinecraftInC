#include <stdlib.h>
#include "raylib.h"
#include "structs.h"
#include "textures.h"
#include "blocks.h"
#include <stdio.h>
#include "mesh.h"

int screenWidth = 1500;
int screenHeight = 1000;

void UpdateDrawFrame(Model model, Camera3D camera_3d);

int main()
{
    BlockDefinition* blocks = initialize_blocks();
    InitWindow(screenWidth, screenHeight, "Minecraft in C");

    SetTargetFPS(60);
    DisableCursor();

    Texture texture = LoadTextures();

    World *world = malloc(sizeof(World));

    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->pos = (IntVector3){0,0,0};

    world->chunks = chunk;

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                int block_id = GetRandomValue(0, 4);
                chunk->blocks[x][y][z] = (Block){ (char)block_id };
            }
        }
    }

    Mesh *mesh = malloc(sizeof(Mesh));

    const int totalBlockCount = 32 * 32 * 32;

    VertexArray* vertexArray = create_vertex_array(48 * 3 * totalBlockCount);

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                Block block = chunk->blocks[x][y][z];

                BlockDefinition blockDefinition = blocks[block.block_id];
                printf("%d \n", blockDefinition.id);

                if(block.block_id != 0){
                    AddBlock((IntVector3){x, y, z}, vertexArray, blockDefinition);
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
    model.materials[0].maps->texture = texture;

    Camera3D camera_3d = { };
    camera_3d.position = (Vector3) { 2, 2, 2};
    camera_3d.target = (Vector3) {0,0,0};
    camera_3d.up = (Vector3) {0, 1, 0};
    camera_3d.fovy = 100;

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera_3d, CAMERA_CUSTOM);
        UpdateDrawFrame(model, camera_3d);
    }

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(const Model model, const Camera3D camera_3d)
{
    BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera_3d);
            DrawModel(model,  (Vector3) { 0,0,1 }, 1, WHITE);
        EndMode3D();

        DrawFPS(50, 50);

    EndDrawing();
}

