#include <string.h>
#include <malloc.h>
#include "structs.h"
#include "textures.h"
#include "blocks.h"

BlockDefinition Air = {};
BlockDefinition JoaChoco = {};
BlockDefinition JoaMini = {};
BlockDefinition JoaSchool = {};
BlockDefinition JoaTruck = {};

BlockDefinition* blockDefinitions;

const int blockCount = 5;

void initialize_blocks(){
    blockDefinitions = malloc(sizeof(BlockDefinition) * blockCount);

    Air.id = 0;
    strcpy(Air.name, "Air");
    blockDefinitions[0] = Air;

    JoaChoco.id = 1;
    strcpy(JoaChoco.name, "JoaChoco");
    all_texture(&JoaChoco, joaChocoTexture);
    blockDefinitions[1] = JoaChoco;

    JoaMini.id = 2;
    strcpy(JoaMini.name, "JoaMini");
    all_texture(&JoaMini, joaMiniTexture);
    blockDefinitions[2] = JoaMini;

    JoaSchool.id = 3;
    strcpy(JoaSchool.name, "JoaSchool");
    all_texture(&JoaSchool, joaSchoolTexture);
    blockDefinitions[3] = JoaSchool;

    JoaTruck.id = 4;
    strcpy(JoaTruck.name, "JoaTruck");
    all_texture(&JoaTruck, joaTruckTexture);
    blockDefinitions[4] = JoaTruck;
}

void all_texture(BlockDefinition* blockDefinition, BlockTexture* blockTexture){
    blockDefinition->leftTexture = blockTexture;
    blockDefinition->rightTexture = blockTexture;
    blockDefinition->closeTexture = blockTexture;
    blockDefinition->awayTexture = blockTexture;
    blockDefinition->topTexture = blockTexture;
    blockDefinition->bottomTexture = blockTexture;
}
