#ifndef BLOCKS_H
#define BLOCKS_H

#include "structs.h"

extern BlockDefinition Air;
extern BlockDefinition JoaChoco;
extern BlockDefinition JoaMini;
extern BlockDefinition JoaSchool;
extern BlockDefinition JoaTruck;

extern const int blockCount;

BlockDefinition* initialize_blocks();
void all_texture(BlockDefinition* blockDefinition, BlockTexture blockTexture);

#endif
