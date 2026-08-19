/* distance3d @ 0x836A7728 — Euclidean distance between two 3D points. */

#include <math.h>

#include "headers/real_point3d.h"

/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float distance3d(const real_point3d *a, const real_point3d *b)
{
    float dx = b->n[0] - a->n[0];
    float dy = b->n[1] - a->n[1];
    float dz = b->n[2] - a->n[2];
    return sqrtf(dx * dx + dy * dy + dz * dz);
}
