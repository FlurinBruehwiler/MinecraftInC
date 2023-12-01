#include <stdlib.h>
#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

Vector3 bottomLeftFront = (Vector3) {0,0,0};
Vector3 bottomRightFront = (Vector3) {0,0,0};
Vector3 bottomLeftBack = (Vector3) {0,0,0};
Vector3 bottomRightBack = (Vector3) {0,0,0};
Vector3 topLeftFront = (Vector3) {0,0,0};
Vector3 topRightFront = (Vector3) {0,0,0};
Vector3 topLeftBack = (Vector3) {0,0,0};
Vector3 topRightBack = (Vector3) {0,0,0};

void SetVertex(float* vertices, int* pos, float x, float y, float z);
void UpdateDrawFrame(Model model, Camera3D camera_3d);
void AddQuadFor(Vector3 pos, float* vertices, enum BlockFace block_face);

enum BlockFace {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    BACK,
    FRONT
};

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    Mesh *mesh = malloc(sizeof *mesh);
    float* vertices = malloc(8 * 3 * sizeof *vertices);
    int pos1 = 0;
    int* pos = &pos1;

    SetVertex(vertices, pos, 0, 0, 0);
    SetVertex(vertices, pos, 1, 0, 0);
    SetVertex(vertices, pos, 1, 0, 1);

    AddQuadFor((Vector3) {0,0,0}, vertices, TOP);

    mesh->vertexCount = 3;
    mesh->triangleCount = 1;
    mesh->vertices = vertices;

    UploadMesh(mesh, false);
    const Model model = LoadModelFromMesh(*mesh);

    Camera3D camera_3d = { };
    camera_3d.position = (Vector3) { 0, 0, 0};
    camera_3d.target = (Vector3) {0,0,1};
    camera_3d.up = (Vector3) {0, 1, 0};
    camera_3d.fovy = 100;

    while (!WindowShouldClose())
    {
        UpdateDrawFrame(model, camera_3d);
    }

    CloseWindow();

    return 0;
}

void SetVertex(float* vertices, int* pos, const float x, const float y, const float z)
{
    vertices[*pos + 0] = x;
    vertices[*pos + 1] = y;
    vertices[*pos + 2] = z;
    *pos += 3;
}

void UpdateDrawFrame(const Model model, const Camera3D camera_3d)
{
    BeginDrawing();

        ClearBackground(WHITE);

        BeginMode3D(camera_3d);
            DrawModel(model,  (Vector3) { 0,0,1 }, 1, RED);
        EndMode3D();

    EndDrawing();
}

void AddQuadFor(Vector3 pos, float* vertices, enum BlockFace block_face, int* floatPos)
{
    SetVertex(vertices, floatPos, )
}

void AddQuad()
{

}
