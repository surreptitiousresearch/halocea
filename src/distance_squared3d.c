#include "headers/real_point3d.h"

/* distance_squared3d — squared Euclidean distance between two 3D points. */
float distance_squared3d(const real_point3d *a, const real_point3d *b)
{
    float dz = b->n[2] - a->n[2];
    float dy = b->n[1] - a->n[1];
    float dx = b->n[0] - a->n[0];
    return (dy * dy) + ((dx * dx) + (dz * dz));
}
