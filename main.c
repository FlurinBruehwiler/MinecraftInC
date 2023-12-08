#include <stdlib.h>
#include "raylib.h"

typedef enum {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    AWAY,
    CLOSE
} BlockFace;

int screenWidth = 1500;
int screenHeight = 1000;

Vector3 bottomLeftAway = (Vector3) {1,0,0};
Vector3 bottomRightAway = (Vector3) {1,0,1};
Vector3 bottomLeftClose = (Vector3) {0,0,0};
Vector3 bottomRightClose = (Vector3) {0,0,1};

Vector3 topLeftAway = (Vector3) {1,1,0};
Vector3 topRightAway = (Vector3) {1,1,1};
Vector3 topRightClose = (Vector3) {0,1,1};
Vector3 topLeftClose = (Vector3) {0,1,0};

typedef struct Vertex {
    Vector3 pos;
    Vector2 textCoord;
} Vertex;

typedef struct VertexArray {
    Vertex* array;
    int pos;
} VertexArray;

typedef struct IntVector3{
    int x;
    int y;
    int z;
} IntVector3;

typedef struct Block {
    char block_id;
} Block;

typedef struct Chunk {
    IntVector3 pos;
    Block blocks[32][32][32];
} Chunk;

typedef struct World {
    Chunk* chunks;
} World;

void UpdateDrawFrame(Model model, Camera3D camera_3d);
void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face);
void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight);
void AddBlock(IntVector3 pos, VertexArray* floatArray);

struct VertexArray* create_vertex_array(int capacity) {
    struct VertexArray* a = malloc(sizeof(VertexArray));
    a->array = malloc(capacity * sizeof(float));
    a->pos = 0;
    return a;
}

void add_vertex(VertexArray* float_array, Vector3 pos, Vector2 texCoord) {
    float_array->array[float_array->pos] = (Vertex){pos, texCoord};
    float_array->pos++;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    DisableCursor();

    Texture2D texture = LoadTexture("joa.png");

    World *world = malloc(sizeof *world);

    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->pos = (IntVector3){0,0,0};

    world->chunks = chunk;

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                int block_id = GetRandomValue(0, 1);
                chunk->blocks[x][y][z] = (Block){ block_id };
            }
        }
    }

    Mesh *mesh = malloc(sizeof *mesh);

    const int blockCount = 32 * 32 * 32;

    VertexArray* vertexArray = create_vertex_array(48 * 3 * blockCount);

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                Block block = chunk->blocks[x][y][z];
                if(block.block_id != 0){
                    AddBlock((IntVector3){x, y, z}, vertexArray);
                }
            }
        }
    }

    float* vertices = malloc(sizeof(float) * vertexArray->pos * 3);
    float* texcoords = malloc(sizeof(float) * vertexArray->pos * 2);

    for (int i = 0; i < vertexArray->pos - 1; ++i) {

        Vertex vertex = vertexArray->array[i];

        vertices[i * 3] = vertex.pos.x;
        vertices[i * 3 + 1] = vertex.pos.y;
        vertices[i * 3 + 2] = vertex.pos.z;

        texcoords[i * 2] = vertex.textCoord.x;
        texcoords[i * 2 + 1] = vertex.textCoord.y;
    }

    mesh->vertexCount = vertexArray->pos;
    mesh->triangleCount = vertexArray->pos;
    mesh->vertices = vertices;
    mesh->texcoords = texcoords;

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

void AddBlock(IntVector3 pos, VertexArray* floatArray)
{
    AddQuadFor(pos, floatArray, LEFT);
    AddQuadFor(pos, floatArray, RIGHT);
    AddQuadFor(pos, floatArray, TOP);
    AddQuadFor(pos, floatArray, BOTTOM);
    AddQuadFor(pos, floatArray, AWAY);
    AddQuadFor(pos, floatArray, CLOSE);
}

void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face)
{
    switch (block_face)
    {
        case LEFT:
            AddQuad(pos, vertices, topLeftAway, topLeftClose, bottomLeftAway, bottomLeftClose);
            break;
        case RIGHT:
            AddQuad(pos, vertices, topRightClose, topRightAway, bottomRightClose, bottomRightAway);
            break;
        case TOP:
            AddQuad(pos, vertices, topLeftAway, topRightAway, topLeftClose, topRightClose);
            break;
        case BOTTOM:
            AddQuad(pos, vertices, bottomLeftClose, bottomRightClose, bottomLeftAway, bottomRightAway);
            break;
        case AWAY:
            AddQuad(pos, vertices, topRightAway, topLeftAway, bottomRightAway, bottomLeftAway);
            break;
        case CLOSE:
            AddQuad(pos, vertices, topLeftClose, topRightClose, bottomLeftClose, bottomRightClose);
            break;
    }
}

Vector3 AddVector(IntVector3 vec1, Vector3 vec2)
{
    Vector3 a = { vec1.x + vec2.x,vec1.y + vec2.y,vec1.z + vec2.z };
    return a;
}

void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight)
{
    add_vertex(vertices, AddVector(pos, topLeft), (Vector2){0, 0});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){0, 1});
    add_vertex(vertices, AddVector(pos, topRight), (Vector2){1, 0});

    add_vertex(vertices, AddVector(pos, topRight), (Vector2){1, 0});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){0, 1});
    add_vertex(vertices, AddVector(pos, bottomRight), (Vector2){1, 1});
}
