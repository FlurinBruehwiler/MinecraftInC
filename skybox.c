#include "mesh.h"
#include "rlgl.h"

Model skybox;

void InitializeSkybox(){
    Mesh cube = GenMeshCube(1,1,1);
    skybox = LoadModelFromMesh(cube);

    Shader shader = LoadShader("shaders/skybox.vs", "shaders/skybox.fs");

    skybox.materials[0].shader = shader;

    int doGamma[] = { 0 };
    int vflipped[] = { 0 };
    int environmentMap[] = { (int)MATERIAL_MAP_CUBEMAP };

    SetShaderValue(shader, GetShaderLocation(shader, "environmentMap"),  environmentMap , SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "doGamma"),  doGamma, SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "vflipped"), vflipped, SHADER_UNIFORM_INT);

    Image img = LoadImage("textures/skybox.png");
    skybox.materials[0].maps[(int)MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);
}

void DrawSkybox(){
    rlDisableBackfaceCulling();
    rlDisableDepthMask();

    DrawModel(skybox, (Vector3){0, 0,0}, 1, WHITE);

    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}
