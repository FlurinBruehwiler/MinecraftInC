#ifndef STRUCTS_H
#define STRUCTS_H

#include "raylib.h"

typedef enum {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    AWAY,
    CLOSE
} BlockFace;

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
    bool isDirty;
    Model model;
    Mesh* mesh;
    bool isEmptyChunk;
} Chunk;

typedef struct BlockTexture {
    const char* name;
    Vector2 topLeft;
    Vector2 bottomRight;
} BlockTexture;

typedef struct BlockDefinition {
    int id;
    char name[20];
    BlockTexture* leftTexture;
    BlockTexture* rightTexture;
    BlockTexture* topTexture;
    BlockTexture* bottomTexture;
    BlockTexture* awayTexture;
    BlockTexture* closeTexture;
} BlockDefinition;

typedef struct RaycastResult {
    IntVector3 hitBlock;
    IntVector3 previousBlock;
    float distance;
    bool hasHit;
} RayCastResult;

#endif
