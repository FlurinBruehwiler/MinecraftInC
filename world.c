#include <stdbool.h>
#include "structs.h"

Chunk chunks[10][10][10];

void InitWorld()
{
    chunks[0][0][0].pos = (IntVector3){0, 0, 0};

    for (int x = 0; x < 32; ++x) {
        for (int y = 0; y < 32; ++y) {
            for (int z = 0; z < 32; ++z) {
                int block_id = GetRandomValue(1, 4);
                chunks[0][0][0].blocks[x][y][z] = (Block){(char)block_id };
            }
        }
    }
}

bool GetChunkOfBlock(IntVector3 pos, Chunk* chunk)
{
    int chunkPosX = pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32;
    int chunkPosY = pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32;
    int chunkPosZ = pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32;

    if(chunkPosX < 0 || chunkPosY < 0 || chunkPosZ < 0 || chunkPosX > 9 || chunkPosY > 9 || chunkPosZ > 9){
        return false;
    }

    *chunk = chunks[chunkPosX][chunkPosY][chunkPosZ];
    return true;
}

bool GetBlockAtPos(IntVector3 pos, Block* block)
{
    int chunkPosX = pos.x >= 0 ? pos.x / 32 : (pos.x - 31) / 32;
    int chunkPosY = pos.y >= 0 ? pos.y / 32 : (pos.y - 31) / 32;
    int chunkPosZ = pos.z >= 0 ? pos.z / 32 : (pos.z - 31) / 32;
    int blockPosX = pos.x >= 0 ? pos.x % 32 : 32 + (pos.x % 32);
    int blockPosY = pos.y >= 0 ? pos.y % 32 : 32 + (pos.y % 32);
    int blockPosZ = pos.z >= 0 ? pos.z % 32 : 32 + (pos.z % 32);

    if(chunkPosX < 0 || chunkPosY < 0 || chunkPosZ < 0 || chunkPosX > 9 || chunkPosY > 9 || chunkPosZ > 9){
        return false;
    }

    *block = chunks[chunkPosX][chunkPosY][chunkPosZ].blocks[blockPosX][blockPosY][blockPosZ];
    return true;
}

IntVector3 LocalToGlobal(Chunk* chunk, IntVector3 localPos)
{
    return (IntVector3){
            chunk->pos.x + localPos.x,
            chunk->pos.y + localPos.y,
            chunk->pos.z + localPos.z,
    };
}

