/* dot_product4d @0x83711550 */
#include "headers/real_vector4d.h"

float dot_product4d(const real_vector4d *a, const real_vector4d *b)
{
    return a->n[0] * b->n[0] + a->n[1] * b->n[1] + a->n[2] * b->n[2] + a->n[3] * b->n[3];
}
