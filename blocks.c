#include <string.h>
#include <malloc.h>
#include "structs.h"
#include "textures.h"
#include "blocks.h"

BlockDefinition Air = {};
BlockDefinition Grass = {};
BlockDefinition Dirt = {};

BlockDefinition* blockDefinitions;

const int blockCount = 5;

void initialize_blocks(){
    blockDefinitions = malloc(sizeof(BlockDefinition) * blockCount);

    Air.id = 0;
    strcpy(Air.name, "Air");
    blockDefinitions[0] = Air;
//
//    CmiBlock.id = 1;
//    strcpy(CmiBlock.name, "CMI");
//    all_texture(&CmiBlock, cmiTexture);
//    blockDefinitions[1] = CmiBlock;

    Grass.id = 1;
    strcpy(Grass.name, "Grass");
    side_texture(&Grass, grassTexture);
    Grass.topTexture = grassTopTexture;
    Grass.bottomTexture = dirtTexture;
    blockDefinitions[1] = Grass;

    Dirt.id = 2;
    strcpy(Dirt.name, "Dirt");
    all_texture(&Dirt, grassTexture);

    blockDefinitions[2] = Dirt;
}

void all_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture){
    blockDefinition->leftTexture = blockTexture;
    blockDefinition->rightTexture = blockTexture;
    blockDefinition->closeTexture = blockTexture;
    blockDefinition->awayTexture = blockTexture;
    blockDefinition->topTexture = blockTexture;
    blockDefinition->bottomTexture = blockTexture;
}

void side_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture){
    blockDefinition->leftTexture = blockTexture;
    blockDefinition->rightTexture = blockTexture;
    blockDefinition->closeTexture = blockTexture;
    blockDefinition->awayTexture = blockTexture;
}
