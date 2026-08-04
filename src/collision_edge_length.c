#include "headers/collision_bsp.h"

extern float __fsqrts(float x);

float collision_edge_length(const collision_bsp *bsp, int edge_index)
{
    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const collision_edge *edge = &((const collision_edge *)bsp->edges.address)[edge_index];

    const float *vertex0 = (const float *)&vertices[edge->vertex_indices[0]];
    const float *vertex1 = (const float *)&vertices[edge->vertex_indices[1]];

    float dx = vertex1[0] - vertex0[0];
    float dy = vertex1[1] - vertex0[1];
    float dz = vertex1[2] - vertex0[2];

    return __fsqrts(dx * dx + dy * dy + dz * dz);
}
