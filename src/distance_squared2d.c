#include "headers/real_point2d.h"

/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float distance_squared2d(const real_point2d *a, const real_point2d *b)
{
    float dx = b->n[0] - a->n[0];
    float dy = b->n[1] - a->n[1];
    return dx * dx + dy * dy;
}
