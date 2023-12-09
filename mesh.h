#ifndef MESH_H
#define MESH_H

#include "structs.h"

void AddBlock(IntVector3 pos, VertexArray* vertexArray, char block_id);
void AddQuadFor(IntVector3 pos, VertexArray* vertices, BlockFace block_face, char block_id);
Vector3 AddVector(IntVector3 vec1, Vector3 vec2);
float map(float value);
void AddQuad(IntVector3 pos, VertexArray* vertices, Vector3 topLeft, Vector3 topRight, Vector3 bottomLeft, Vector3 bottomRight, BlockTexture* blockTexture);
struct VertexArray* create_vertex_array(int capacity);
void add_vertex(VertexArray* float_array, Vector3 pos, Vector2 texCoord);

#endif
