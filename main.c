#include <stdlib.h>
#include "raylib.h"
#include <string.h>
#include <stdio.h>

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

typedef struct BlockTexture {
    const char* name;
    Vector2 topLeft;
    Vector2 bottomRight;
} BlockTexture;

typedef struct BlockDefinition {
    int id;
    char name[20];
    BlockTexture leftTexture;
    BlockTexture rightTexture;
    BlockTexture topTexture;
    BlockTexture bottomTexture;
    BlockTexture awayTexture;
    BlockTexture closeTexture;
} BlockDefinition;

BlockDefinition Air = {};
BlockDefinition JoaChoco = {};
BlockDefinition JoaMini = {};
BlockDefinition JoaSchool = {};
BlockDefinition JoaTruck = {};

BlockTexture joaChocoTexture = {"joaChoco" };
BlockTexture joaMiniTexture = {"joaMini"};
BlockTexture joaSchoolTexture = {"joaSchool"};
BlockTexture joaTruckTexture = {"joaTruck"};

const int blockCount = 5;
int textureWidth = 0;

void UpdateDrawFrame(Model model, Camera3D camera_3d);
void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face, BlockDefinition blockDefinition);
void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight, BlockTexture blockTexture);
void AddBlock(IntVector3 pos, VertexArray* floatArray, BlockDefinition blockDefinition);

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

void all_texture(BlockDefinition* blockDefinition, BlockTexture blockTexture){
    blockDefinition->leftTexture = blockTexture;
    blockDefinition->rightTexture = blockTexture;
    blockDefinition->closeTexture = blockTexture;
    blockDefinition->awayTexture = blockTexture;
    blockDefinition->topTexture = blockTexture;
    blockDefinition->bottomTexture = blockTexture;
}

BlockDefinition* initialize_blocks(){
    BlockDefinition* blockDefinitions = malloc(sizeof(BlockDefinition) * blockCount);

    Air.id = 0;
    strcpy(Air.name, "Air");
    blockDefinitions[0] = Air;

    JoaChoco.id = 1;
    strcpy(JoaChoco.name, "JoaChoco");
    all_texture(&JoaChoco, joaChocoTexture);
    blockDefinitions[1] = JoaChoco;

    JoaMini.id = 2;
    strcpy(JoaMini.name, "JoaMini");
    all_texture(&JoaMini, joaMiniTexture);
    blockDefinitions[2] = JoaMini;

    JoaSchool.id = 3;
    strcpy(JoaSchool.name, "JoaSchool");
    all_texture(&JoaSchool, joaSchoolTexture);
    blockDefinitions[3] = JoaSchool;

    JoaTruck.id = 4;
    strcpy(JoaTruck.name, "JoaTruck");
    all_texture(&JoaTruck, joaTruckTexture);
    blockDefinitions[4] = JoaTruck;

    return blockDefinitions;
}

Texture LoadTextures(){
    const int textureCount = 4;
    BlockTexture blockTexture[textureCount];
    blockTexture[0] = joaChocoTexture;
    blockTexture[1] = joaMiniTexture;
    blockTexture[2] = joaSchoolTexture;
    blockTexture[3] = joaTruckTexture;

    int maxHeight = 0;
    Texture2D textures[textureCount];
    for (int i = 0; i < textureCount; ++i) {
        char result[100];
        strcpy(result, "textures/");
        strcat(result, blockTexture[i].name);
        strcat(result, ".png");
        Texture2D texture = LoadTexture(result);

        if(texture.height > maxHeight){
            maxHeight = texture.height;
        }
        textureWidth += texture.width;

        textures[i] = texture;
    }

    RenderTexture2D target = LoadRenderTexture(textureWidth, maxHeight);

    BeginTextureMode(target);

    int currentWidth = 0;

    for (int i = 0; i < textureCount; ++i) {
        Texture2D texture2D = textures[i];
        DrawTexture(texture2D, currentWidth, 0, WHITE);
        blockTexture[i].topLeft = (Vector2){(float)currentWidth, 0};
        blockTexture[i].bottomRight = (Vector2){(float)currentWidth + (float)texture2D.width, (float)texture2D.height};

        currentWidth += texture2D.width;
    }

    EndTextureMode();

    Image image = LoadImageFromTexture(target.texture);
    ExportImage(image, "textureAtlas.png");

    return target.texture;
}

int main()
{
    BlockDefinition* blocks = initialize_blocks();
    InitWindow(screenWidth, screenHeight, "Minecraft in C");

    SetTargetFPS(60);
    DisableCursor();

    Texture texture = LoadTextures();

    World *world = malloc(sizeof *world);

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

    Mesh *mesh = malloc(sizeof *mesh);

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

void AddBlock(IntVector3 pos, VertexArray* floatArray, BlockDefinition blockDefinition)
{
    AddQuadFor(pos, floatArray, LEFT, blockDefinition);
    AddQuadFor(pos, floatArray, RIGHT, blockDefinition);
    AddQuadFor(pos, floatArray, TOP, blockDefinition);
    AddQuadFor(pos, floatArray, BOTTOM, blockDefinition);
    AddQuadFor(pos, floatArray, AWAY, blockDefinition);
    AddQuadFor(pos, floatArray, CLOSE, blockDefinition);
}

void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face, BlockDefinition blockDefinition)
{
    switch (block_face)
    {
        case LEFT:
            AddQuad(pos, vertices, topLeftAway, topLeftClose, bottomLeftAway, bottomLeftClose, blockDefinition.leftTexture);
            break;
        case RIGHT:
            AddQuad(pos, vertices, topRightClose, topRightAway, bottomRightClose, bottomRightAway, blockDefinition.rightTexture);
            break;
        case TOP:
            AddQuad(pos, vertices, topLeftAway, topRightAway, topLeftClose, topRightClose, blockDefinition.topTexture);
            break;
        case BOTTOM:
            AddQuad(pos, vertices, bottomLeftClose, bottomRightClose, bottomLeftAway, bottomRightAway, blockDefinition.bottomTexture);
            break;
        case AWAY:
            AddQuad(pos, vertices, topRightAway, topLeftAway, bottomRightAway, bottomLeftAway, blockDefinition.awayTexture);
            break;
        case CLOSE:
            AddQuad(pos, vertices, topLeftClose, topRightClose, bottomLeftClose, bottomRightClose, blockDefinition.closeTexture);
            break;
    }
}

Vector3 AddVector(IntVector3 vec1, Vector3 vec2)
{
    Vector3 a = { (float)vec1.x + vec2.x,(float)vec1.y + vec2.y,(float)vec1.z + vec2.z };
    return a;
}

float map(float value) {
    return value / (float)textureWidth;
}

void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight, BlockTexture blockTexture)
{
    float left = map(blockTexture.topLeft.x);
    float right = map(blockTexture.bottomRight.x);
    float bottom = map(blockTexture.bottomRight.y);

    add_vertex(vertices, AddVector(pos, topLeft), (Vector2){left, bottom});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){left, 0});
    add_vertex(vertices, AddVector(pos, topRight), (Vector2){right, bottom});

    add_vertex(vertices, AddVector(pos, topRight), (Vector2){right, bottom});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){left, 0});
    add_vertex(vertices, AddVector(pos, bottomRight), (Vector2){right, 0});
}


