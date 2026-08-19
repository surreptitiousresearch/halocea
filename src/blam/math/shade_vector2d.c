/* shade_vector2d @ 0x836F75E0 — barycentric-style blend of three 2D vectors:
 * result = vertex0 + (vertex1-vertex0)*b + (vertex2-vertex0)*c. (r3=vertex0, r4=vertex1, r5=vertex2, f1=b, f2=c;
 * result pointer lands in r8 because the two float args reserve r6/r7.) */

#include "headers/real_vector2d.h"

void shade_vector2d(const real_vector2d *vertex0, const real_vector2d *vertex1, const real_vector2d *vertex2,
                    float b, float c, real_vector2d *result)
{
    result->n[0] = vertex0->n[0] + (vertex1->n[0] - vertex0->n[0]) * b + (vertex2->n[0] - vertex0->n[0]) * c;
    result->n[1] = vertex0->n[1] + (vertex1->n[1] - vertex0->n[1]) * b + (vertex2->n[1] - vertex0->n[1]) * c;
}
