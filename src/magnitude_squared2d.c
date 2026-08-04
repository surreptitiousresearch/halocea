#include "headers/real_vector2d.h"

float magnitude_squared2d(const real_vector2d *v)
{
    return (v->n[0] * v->n[0] + v->n[1] * v->n[1]);
}
