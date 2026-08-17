/* midpoint3d @0x837AC2F8 */
#include "headers/real_point3d.h"

real_point3d * midpoint3d(const real_point3d *p0, const real_point3d *p1, real_point3d *result)
{
    result->n[0] = (p0->n[0] + p1->n[0]) * 0.5f;
    result->n[1] = (p0->n[1] + p1->n[1]) * 0.5f;
    result->n[2] = (p0->n[2] + p1->n[2]) * 0.5f;
    return result;
}
