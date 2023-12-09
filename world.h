#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include "structs.h"

extern Chunk chunks[10][10][10];

void InitWorld();
bool GetBlockAtPos(IntVector3 pos, Block* block);
IntVector3 LocalToGlobal(Chunk* chunk, IntVector3 localPos);

#endif
