#include <malloc.h>
#include <stdio.h>
#include "structs.h"
#include "mesh.h"
#include "textures.h"
#include "blocks.h"
#include "world.h"

Vector3 bottomLeftAway = (Vector3) {1,0,0};
Vector3 bottomRightAway = (Vector3) {1,0,1};
Vector3 bottomLeftClose = (Vector3) {0,0,0};
Vector3 bottomRightClose = (Vector3) {0,0,1};

Vector3 topLeftAway = (Vector3) {1,1,0};
Vector3 topRightAway = (Vector3) {1,1,1};
Vector3 topRightClose = (Vector3) {0,1,1};
Vector3 topLeftClose = (Vector3) {0,1,0};

void RegenDirtyChunks(){
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            for (int z = 0; z < 10; ++z) {
                if(chunks[x][y][z].isDirty && !chunks[x][y][z].isEmptyChunk){
                    printf("regen chunk \n");
                    chunks[x][y][z].isDirty = false;
                    GenMesh(&chunks[x][y][z]);
                }
            }
        }
    }
}

void GenMesh(Chunk* chunk){
    if(chunk->hasMesh){
        UnloadMesh(*chunk->mesh);
    }

    Mesh *mesh = malloc(sizeof(Mesh));
    *mesh = (Mesh){ };

    VertexArray* vertexArray = create_vertex_array(2 * 3 * 6 * totalBlockCount);

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
    Model model = LoadModelFromMesh(*mesh);

    model.materials[0].maps->texture = textureAtlas;
    chunk->model = model;
    chunk->mesh = mesh;
    chunk->hasMesh = true;
}

bool IsAir(int x, int y, int z){

    Block block;
    if(GetBlockAtPos((IntVector3){x,y,z}, &block)){
        if(block.block_id == 0)
            return true;
        return false;
    }
    return true;
}

void AddBlock(IntVector3 pos, VertexArray* vertexArray, char block_id)
{
    if (IsAir(pos.x, pos.y, pos.z - 1)){
        AddQuadFor(pos, vertexArray, LEFT, block_id);
    }
    if (IsAir(pos.x, pos.y, pos.z + 1)){
        AddQuadFor(pos, vertexArray, RIGHT, block_id);
    }
    if (IsAir(pos.x, pos.y + 1, pos.z)){
        AddQuadFor(pos, vertexArray, TOP, block_id);
    }
    if (IsAir(pos.x, pos.y - 1, pos.z)){
        AddQuadFor(pos, vertexArray, BOTTOM, block_id);
    }
    if (IsAir(pos.x + 1, pos.y, pos.z)){
        AddQuadFor(pos, vertexArray, AWAY, block_id);
    }
    if (IsAir(pos.x - 1, pos.y, pos.z)){
        AddQuadFor(pos, vertexArray, CLOSE, block_id);
    }
}

void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face, char block_id)
{
    BlockDefinition blockDefinition = blockDefinitions[block_id];

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

void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight, BlockTexture* blockTexture)
{
    float left = map(blockTexture->topLeft.x);
    float right = map(blockTexture->bottomRight.x);
    float bottom = map(blockTexture->bottomRight.y * 2);

    add_vertex(vertices, AddVector(pos, topLeft), (Vector2){left, bottom});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){left, 0});
    add_vertex(vertices, AddVector(pos, topRight), (Vector2){right, bottom});

    add_vertex(vertices, AddVector(pos, topRight), (Vector2){right, bottom});
    add_vertex(vertices, AddVector(pos, bottomLeft), (Vector2){left, 0});
    add_vertex(vertices, AddVector(pos, bottomRight), (Vector2){right, 0});
}

VertexArray* create_vertex_array(int capacity) {
    struct VertexArray* a = malloc(sizeof(VertexArray));
    a->array = malloc(capacity * sizeof(float));
    a->pos = 0;
    return a;
}

void add_vertex(VertexArray* float_array, Vector3 pos, Vector2 texCoord) {
    float_array->array[float_array->pos] = (Vertex){pos, texCoord};
    float_array->pos++;
}
