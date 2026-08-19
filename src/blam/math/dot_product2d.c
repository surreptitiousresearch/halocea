/* dot_product2d @0x836FBEB8 */
#include "headers/real_vector2d.h"

/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float dot_product2d(const real_vector2d *a, const real_vector2d *b)
{
    return a->n[0]*b->n[0] + a->n[1]*b->n[1];
}
