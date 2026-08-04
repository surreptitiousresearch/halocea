#include <math.h>
#include "headers/real_vector2d.h"

/* DEVIATION: __fsqrts double-return idiom; replaced with sqrtf. */
float magnitude2d(const real_vector2d *v)
{
    return sqrtf((v->n[0] * v->n[0]) + (v->n[1] * v->n[1]));
}
