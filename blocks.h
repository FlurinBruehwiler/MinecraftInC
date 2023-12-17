#ifndef BLOCKS_H
#define BLOCKS_H

#include "structs.h"

extern BlockDefinition Air;
extern BlockDefinition Grass;
extern BlockDefinition Dirt;

extern BlockDefinition* blockDefinitions;

extern const int blockCount;

void initialize_blocks();

void side_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture);
void all_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture);

#endif
