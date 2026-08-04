/* point_in_sphere @0x836E0BE0 — true if point lies within (or on) the sphere
 * of the given radius centred at center. Compares squared distance to squared
 * radius to avoid a sqrt. */

#include "headers/real_point3d.h"

int point_in_sphere(const real_point3d *point, const real_point3d *center, float radius)
{
    float dx = center->n[0] - point->n[0];
    float dy = center->n[1] - point->n[1];
    float dz = center->n[2] - point->n[2];

    return (dy * dy) + ((dx * dx) + (dz * dz)) <= (double)(radius * radius);
}
