/* dot_product3d @ 0x8369ADF0 — dot product of two 3D vectors. */

#include "headers/real_vector3d.h"

/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float dot_product3d(const real_vector3d *a, const real_vector3d *b)
{
    return a->n[0]*b->n[0] + a->n[1]*b->n[1] + a->n[2]*b->n[2];
}
