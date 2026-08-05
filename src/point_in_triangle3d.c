/* point_in_triangle3d @0x836FE0A0 — test whether `point` lies inside triangle (p0,p1,p2) and, if so,
 * compute its barycentric coordinates (s,t) relative to edges (p1-p0) and (p2-p0). Returns 0 if the
 * point is off the triangle plane (distance check) or outside the triangle; 1 if inside, with *s,*t
 * filled.
 *
 * The triangle is projected onto the coordinate plane most perpendicular to its normal (selected by
 * dominant normal axis + sign via global_projection3d_mappings) and the barycentric weights are
 * solved in 2D. Float groupings preserved from the decompiler. */

#include <stdint.h>
#include "headers/real_point3d.h"

extern const int16_t global_projection3d_mappings[1][6][2];
extern float __fabs(float x);

/* attested: return is uint8_t — both callers re-normalize with clrlwi r3,24
 * (structure_render_surface_from_point_and_leaf @837447E4/@83744894). */
uint8_t point_in_triangle3d(const real_point3d *point, const real_point3d *p0, const real_point3d *p1,
        const real_point3d *p2, float *s, float *t)
{
    float edge1[3];
    float edge2[3];
    float pvec[3];
    float normal[3];
    float plane_dist;
    float abs_n0, abs_n1, abs_n2;
    int axis;
    int slot, keep0, keep1;
    float numerator_s, numerator_t, denominator, inv;

    edge1[0] = p1->n[0] - p0->n[0];
    edge1[1] = p1->n[1] - p0->n[1];
    edge1[2] = p1->n[2] - p0->n[2];

    edge2[0] = p2->n[0] - p0->n[0];
    edge2[1] = p2->n[1] - p0->n[1];
    edge2[2] = p2->n[2] - p0->n[2];

    pvec[0] = point->n[0] - p0->n[0];
    pvec[1] = point->n[1] - p0->n[1];
    pvec[2] = point->n[2] - p0->n[2];

    normal[0] = (edge1[1] * edge2[2]) - (edge2[1] * edge1[2]);
    normal[1] = (edge1[2] * edge2[0]) - (edge2[2] * edge1[0]);
    normal[2] = (edge2[1] * edge1[0]) - (edge1[1] * edge2[0]);

    plane_dist = ((normal[0] * pvec[0])
               + ((normal[2] * pvec[2]) + (normal[1] * pvec[1])));

    /* reject if the point is too far from the triangle's plane (squared distance vs |normal|^2 * eps) */
    if ( (plane_dist * plane_dist) >= (double)(((normal[0] * normal[0])
            + ((normal[2] * normal[2]) + (normal[1] * normal[1])))
            * (float)0.000099999997) )
        return 0;

    /* dominant axis of the normal == projection_from_vector3d(normal) */
    abs_n2 = __fabs(normal[2]);
    abs_n1 = __fabs(normal[1]);
    abs_n0 = __fabs(normal[0]);
    if ( abs_n2 < abs_n1 || abs_n2 < abs_n0 )
        axis = abs_n1 >= abs_n0;        /* dominant is axis 0 or 1 */
    else
        axis = 2;                       /* dominant is axis 2 */

    slot = 2 * axis + (normal[axis] > 0.0);
    keep0 = global_projection3d_mappings[0][slot][0];
    keep1 = global_projection3d_mappings[0][slot][1];

    /* barycentric numerators/denominator in the projected 2D plane */
    numerator_t = (pvec[keep1] * edge1[keep0]) - (edge1[keep1] * pvec[keep0]);
    numerator_s = (edge2[keep1] * pvec[keep0]) - (pvec[keep1] * edge2[keep0]);
    denominator = (edge2[keep1] * edge1[keep0]) - (edge1[keep1] * edge2[keep0]);

    if ( numerator_t < 0.0
      || numerator_s < 0.0
      || (numerator_s + numerator_t) > (double)denominator )
        return 0;

    inv = ((float)1.0 / denominator);
    *s = inv * numerator_s;
    *t = inv * numerator_t;
    return 1;
}
