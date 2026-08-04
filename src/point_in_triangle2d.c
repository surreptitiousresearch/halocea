/* point_in_triangle2d @0x836FDFF8 — barycentric point-in-triangle test for the
 * triangle (p0,p1,p2). Returns 1 and writes barycentric coordinates (s,t) when
 * point is inside, 0 otherwise. */

#include "headers/real_point2d.h"

int point_in_triangle2d(const real_point2d *point, const real_point2d *p0, const real_point2d *p1, const real_point2d *p2, float *s, float *t)
{
    /* Edge cross products (signed areas x2). */
    float area_p1 = ((p1->n[0] - p0->n[0]) * (point->n[1] - p0->n[1]))
                  - ((point->n[0] - p0->n[0]) * (p1->n[1] - p0->n[1]));
    float area_p2 = ((point->n[0] - p0->n[0]) * (p2->n[1] - p0->n[1]))
                  - ((p2->n[0] - p0->n[0]) * (point->n[1] - p0->n[1]));
    float double_area = ((p1->n[0] - p0->n[0]) * (p2->n[1] - p0->n[1]))
                      - ((p2->n[0] - p0->n[0]) * (p1->n[1] - p0->n[1]));
    float inverse_double_area;

    if (area_p1 < 0.0
        || area_p2 < 0.0
        || (area_p2 + area_p1) > (double)double_area)
    {
        return 0;
    }

    inverse_double_area = 1.0f / double_area;
    *s = inverse_double_area * area_p2;
    *t = inverse_double_area * area_p1;
    return 1;
}
