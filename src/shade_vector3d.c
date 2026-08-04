/* shade_vector3d @ 0x836F7630 — barycentric-style blend of three 3D vectors:
 * result = vertex0 + (vertex1-vertex0)*b + (vertex2-vertex0)*c. (r3=vertex0, r4=vertex1, r5=vertex2, f1=b, f2=c;
 * result pointer lands in r8 because the two float args reserve r6/r7.) */

#include "headers/real_vector3d.h"

void shade_vector3d(const real_vector3d *vertex0, const real_vector3d *vertex1, const real_vector3d *vertex2,
                    float b, float c, real_vector3d *result)
{
    result->n[0] = vertex0->n[0] + (vertex1->n[0] - vertex0->n[0]) * b + (vertex2->n[0] - vertex0->n[0]) * c;
    result->n[1] = vertex0->n[1] + (vertex1->n[1] - vertex0->n[1]) * b + (vertex2->n[1] - vertex0->n[1]) * c;
    result->n[2] = vertex0->n[2] + (vertex1->n[2] - vertex0->n[2]) * b + (vertex2->n[2] - vertex0->n[2]) * c;
}
