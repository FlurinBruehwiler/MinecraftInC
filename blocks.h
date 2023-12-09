#ifndef BLOCKS_H
#define BLOCKS_H

#include "structs.h"

extern BlockDefinition Air;
extern BlockDefinition JoaChoco;
extern BlockDefinition JoaMini;
extern BlockDefinition JoaSchool;
extern BlockDefinition JoaTruck;

extern BlockDefinition* blockDefinitions;

extern const int blockCount;

void initialize_blocks();
void all_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture);

#endif
