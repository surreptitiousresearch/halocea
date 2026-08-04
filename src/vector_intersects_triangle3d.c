/* vector_intersects_triangle3d @0x836FF090 — Moller-Trumbore-style ray/segment-vs-triangle intersection
 * test. `point` + s*`vector` (s in the barycentric sense below) against triangle (p0,p1,p2). On success,
 * writes the p0-relative barycentric "u" weight to *t_reference and returns 1; returns 0 for a
 * near-parallel vector, an out-of-range u/v, or a failing final combined-weight check.
 *
 * DEVIATION: the decompiler fully inlines every cross/dot product into one giant nested expression per
 * line; algebraically factored into named edge vectors and two cross products (verified term-by-term
 * against the original grouping) for readability — no change in the arithmetic performed. Per disasm/
 * decompile, *t_reference is written the same barycentric-u expression used for the first range check, not
 * a distinct ray parameter "t" — reproduced as such rather than assumed to be a bug. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fabs(float x);

int vector_intersects_triangle3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *p0,
    const real_point3d *p1, const real_point3d *p2, float *t_reference)
{
    real_vector3d edge1, edge2, tvec, cross_edge1_edge2, cross_vector_tvec;

    edge1.n[0] = p1->n[0] - p0->n[0];
    edge1.n[1] = p1->n[1] - p0->n[1];
    edge1.n[2] = p1->n[2] - p0->n[2];

    edge2.n[0] = p2->n[0] - p0->n[0];
    edge2.n[1] = p2->n[1] - p0->n[1];
    edge2.n[2] = p2->n[2] - p0->n[2];

    tvec.n[0] = p0->n[0] - point->n[0];
    tvec.n[1] = p0->n[1] - point->n[1];
    tvec.n[2] = p0->n[2] - point->n[2];

    cross_edge1_edge2.n[0] = edge1.n[1] * edge2.n[2] - edge1.n[2] * edge2.n[1];
    cross_edge1_edge2.n[1] = edge1.n[2] * edge2.n[0] - edge1.n[0] * edge2.n[2];
    cross_edge1_edge2.n[2] = edge1.n[0] * edge2.n[1] - edge1.n[1] * edge2.n[0];

    float det = vector->n[0] * cross_edge1_edge2.n[0] + vector->n[1] * cross_edge1_edge2.n[1]
              + vector->n[2] * cross_edge1_edge2.n[2];

    if ( __fabs(det) < 0.000099999997f )
        return 0;

    float inv_det = 1.0f / det;

    float u = (tvec.n[0] * cross_edge1_edge2.n[0] + tvec.n[1] * cross_edge1_edge2.n[1]
             + tvec.n[2] * cross_edge1_edge2.n[2]) * inv_det;
    if ( u < 0.0f || u > 1.0f )
        return 0;

    cross_vector_tvec.n[0] = vector->n[1] * tvec.n[2] - vector->n[2] * tvec.n[1];
    cross_vector_tvec.n[1] = vector->n[2] * tvec.n[0] - vector->n[0] * tvec.n[2];
    cross_vector_tvec.n[2] = vector->n[0] * tvec.n[1] - vector->n[1] * tvec.n[0];

    float v = -(cross_vector_tvec.n[0] * edge2.n[0] + cross_vector_tvec.n[1] * edge2.n[1]
              + cross_vector_tvec.n[2] * edge2.n[2]) * inv_det;
    if ( v < 0.0f || v > 1.0f )
        return 0;

    float w = -(cross_vector_tvec.n[0] * edge1.n[0] + cross_vector_tvec.n[1] * edge1.n[1]
              + cross_vector_tvec.n[2] * edge1.n[2]) * inv_det;
    if ( -w >= 0.0f && -w + v <= 1.0f )
    {
        *t_reference = u;
        return 1;
    }

    return 0;
}
