#include <stdbool.h>
#include <malloc.h>
#include "structs.h"
#include "player.h"
#include "world.h"
#include "perlin.h"

Chunk** chunks;

const int totalBlockCount = 32 * 32 * 32;
const int chunkCount = 32 * 32 * 32;

int GetChunkIndex(int x, int y, int z){
    x += 16;
    y += 16;
    z += 16;
    return (x) + (y * 32) + (z * 32 * 32);
}

Chunk* GetChunk(int x, int y, int z){
    return chunks[GetChunkIndex(x, y, z)];
}

bool ChunkIsLoaded(int x, int y, int z)
{
    return chunks[GetChunkIndex(x, y, z)];
}

const float scale = 0.025f;

void GenChunk(int chunkX, int chunkY, int chunkZ){
    Chunk* chunk = malloc(sizeof(Chunk));
    chunk->pos = (IntVector3){chunkX, chunkY, chunkZ};
    chunk->isDirty = true;
    chunk->isEmptyChunk = false;
    chunk->mesh = NULL;
    chunks[GetChunkIndex(chunkX, chunkY, chunkZ)] = chunk;

    for (int x = 0; x < 32; ++x) {
        for (int z = 0; z < 32; ++z) {

            int globalX = x + (chunkX * 32);
            int globalZ = z + (chunkZ * 32);



            double res = OctavePerlin(((float)globalX + 100000.0f) * scale, 0, ((float)globalZ + 100000.0f) * scale, 1, 2);
            int height = (int)(res * 32 * 3);

            for (int y = 0; y < 32; ++y) {
                Block block;
                if(chunkY * 32 + y > height){
                    block = (Block){(char)0 };
                }
                else if(chunkY * 32 + y == height){
                    block = (Block){(char)1 };
                }else{
                    block = (Block){(char)2 };
                }
                chunk->blocks[x][y][z] = block;
            }
        }
    }
}

void GenChunksInRenderDistance()
{
    const int renderDistance = 8;
    IntVector3 chunkPos = GetChunkPos((IntVector3){(int)playerCam.position.x, (int)playerCam.position.y, (int)playerCam.position.z});

    for (int x = -renderDistance; x < renderDistance; x++)
    {
        for (int z = -renderDistance; z < renderDistance; z++)
        {
            for (int y = 0; y < 3; y++)
            {
                IntVector3 neededChunk = (IntVector3){chunkPos.x + x, y, chunkPos.z + z};

                if(!ChunkIsLoaded(neededChunk.x, neededChunk.y, neededChunk.z)){
                    GenChunk(neededChunk.x, neededChunk.y, neededChunk.z);
                    return;
                }
            }
        }
    }
}

void InitWorld()
{
    chunks = malloc(sizeof(Chunk*) * chunkCount);
    for (int i = 0; i < chunkCount; ++i) {
        chunks[i] = NULL;
    }
}

bool GetChunkOfBlock(IntVector3 pos, Chunk** chunk)
{
    int chunkPosX = pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32;
    int chunkPosY = pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32;
    int chunkPosZ = pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32;

    if(!ChunkIsLoaded(chunkPosX, chunkPosY, chunkPosZ))
        return false;

    *chunk = GetChunk(chunkPosX,chunkPosY,chunkPosZ);
    return true;
}

bool GetBlockPtrAtPos(IntVector3 pos, Block** block)
{
    int chunkPosX = pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32;
    int chunkPosY = pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32;
    int chunkPosZ = pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32;

// Ensure block coordinates are positive within the chunk
    int blockPosX = (pos.x >= 0 ? pos.x % 32 : (pos.x % 32 + 32)) % 32;
    int blockPosY = (pos.y >= 0 ? pos.y % 32 : (pos.y % 32 + 32)) % 32;
    int blockPosZ = (pos.z >= 0 ? pos.z % 32 : (pos.z % 32 + 32)) % 32;


    if(!ChunkIsLoaded(chunkPosX, chunkPosY, chunkPosZ))
        return false;

    *block = &GetChunk(chunkPosX, chunkPosY, chunkPosZ)->blocks[blockPosX][blockPosY][blockPosZ];
    return true;
}

bool GetBlockAtPos(IntVector3 pos, Block* block)
{
    int chunkPosX = pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32;
    int chunkPosY = pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32;
    int chunkPosZ = pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32;

// Ensure block coordinates are positive within the chunk
    int blockPosX = (pos.x >= 0 ? pos.x % 32 : (pos.x % 32 + 32)) % 32;
    int blockPosY = (pos.y >= 0 ? pos.y % 32 : (pos.y % 32 + 32)) % 32;
    int blockPosZ = (pos.z >= 0 ? pos.z % 32 : (pos.z % 32 + 32)) % 32;


    if(!ChunkIsLoaded(chunkPosX, chunkPosY, chunkPosZ))
        return false;

    *block = GetChunk(chunkPosX,chunkPosY,chunkPosZ)->blocks[blockPosX][blockPosY][blockPosZ];
    return true;
}

IntVector3 LocalToGlobal(Chunk* chunk, IntVector3 localPos)
{
    return (IntVector3){
            chunk->pos.x * 32 + localPos.x,
            chunk->pos.y * 32 + localPos.y,
            chunk->pos.z * 32 + localPos.z,
    };
}

IntVector3 GetChunkPos(IntVector3 pos)
{
    return (IntVector3){
            pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32,
            pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32,
            pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32,
    };
}
