#include "headers/real_vector3d.h"

/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float magnitude_squared3d(const real_vector3d *v)
{
    return v->n[0]*v->n[0] + v->n[1]*v->n[1] + v->n[2]*v->n[2];
}
