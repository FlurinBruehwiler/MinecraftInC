#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include "structs.h"

extern Chunk** chunks;
extern const int totalBlockCount;
extern const int chunkCount;

int GetChunkIndex(int x, int y, int z);
Chunk* GetChunk(int x, int y, int z);
void GenChunk(int chunkX, int chunkY, int chunkZ);
void InitWorld();
bool GetBlockAtPos(IntVector3 pos, Block* block);
bool GetBlockPtrAtPos(IntVector3 pos, Block** block);
IntVector3 LocalToGlobal(Chunk* chunk, IntVector3 localPos);
bool GetChunkOfBlock(IntVector3 pos, Chunk** chunk);

#endif
