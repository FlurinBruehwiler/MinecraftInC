#include "raylib.h"
#include "structs.h"
#include <string.h>
#include "textures.h"

BlockTexture joaChocoTexture = {"joaChoco"};
BlockTexture joaMiniTexture = {"joaMini"};
BlockTexture joaSchoolTexture = {"joaSchool"};
BlockTexture joaTruckTexture = {"joaTruck"};

int textureWidth = 0;

Texture LoadTextures() {
    const int textureCount = 4;
    BlockTexture blockTexture[textureCount];
    blockTexture[0] = joaChocoTexture;
    blockTexture[1] = joaMiniTexture;
    blockTexture[2] = joaSchoolTexture;
    blockTexture[3] = joaTruckTexture;

    int maxHeight = 0;
    Texture2D textures[textureCount];
    for (int i = 0; i < textureCount; ++i) {
        char result[100];
        strcpy(result, "textures/");
        strcat(result, blockTexture[i].name);
        strcat(result, ".png");
        Texture2D texture = LoadTexture(result);

        if (texture.height > maxHeight) {
            maxHeight = texture.height;
        }
        textureWidth += texture.width;

        textures[i] = texture;
    }

    RenderTexture2D target = LoadRenderTexture(textureWidth, maxHeight);

    BeginTextureMode(target);

    int currentWidth = 0;

    for (int i = 0; i < textureCount; ++i) {
        Texture2D texture2D = textures[i];
        DrawTexture(texture2D, currentWidth, 0, WHITE);
        blockTexture[i].topLeft = (Vector2) {(float) currentWidth, 0};
        blockTexture[i].bottomRight = (Vector2) {(float) currentWidth + (float) texture2D.width,
                                                 (float) texture2D.height};

        currentWidth += texture2D.width;
    }

    EndTextureMode();

    Image image = LoadImageFromTexture(target.texture);
    ExportImage(image, "textureAtlas.png");

    return target.texture;
}
