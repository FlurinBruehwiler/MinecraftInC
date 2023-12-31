#include "raylib.h"
#include "structs.h"
#include <string.h>
#include <malloc.h>
#include "textures.h"

BlockTexture* dirtTexture;
BlockTexture* grassTexture;
BlockTexture* grassTopTexture;

Texture textureAtlas;

int textureWidth = 0;
int textureHeight = 0;

void InitializeTextureDefinitions();

void LoadTextureAtlas() {
    InitializeTextureDefinitions();

    const int textureCount = 3;
    BlockTexture* blockTexture[textureCount];
    blockTexture[0] = dirtTexture;
    blockTexture[1] = grassTexture;
    blockTexture[2] = grassTopTexture;

    Texture2D textures[textureCount];
    for (int i = 0; i < textureCount; ++i) {
        char result[100];
        strcpy(result, "textures/");
        strcat(result, blockTexture[i]->name);
        strcat(result, ".png");
        Texture2D texture = LoadTexture(result);

        if (texture.height > textureHeight) {
            textureHeight = texture.height;
        }
        textureWidth += texture.width;

        textures[i] = texture;
    }

    RenderTexture2D target = LoadRenderTexture(textureWidth, textureHeight);

    BeginTextureMode(target);

    int currentWidth = 0;

    for (int i = 0; i < textureCount; ++i) {
        Texture2D texture2D = textures[i];
        DrawTexture(texture2D, currentWidth, textureHeight - texture2D.height, WHITE);
        blockTexture[i]->topLeft = (Vector2) {(float) currentWidth, 0};
        blockTexture[i]->bottomRight = (Vector2) {(float) currentWidth + (float) texture2D.width,
                                                 (float) texture2D.height};

        currentWidth += texture2D.width;
    }

    EndTextureMode();

    Image image = LoadImageFromTexture(target.texture);
    ExportImage(image, "textureAtlas.png");

    textureAtlas = target.texture;
}

void InitializeTextureDefinitions(){
    dirtTexture = malloc(sizeof(BlockTexture));
    dirtTexture->name = "dirt";

    grassTexture = malloc(sizeof(BlockTexture));
    grassTexture->name = "grass";

    grassTopTexture = malloc(sizeof(BlockTexture));
    grassTopTexture->name = "grasstop";
}
