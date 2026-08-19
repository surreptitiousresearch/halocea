/* triple_product3d @ 0x836FC068
   Scalar triple product n . (a x b), computed component-wise in single precision. */
#include "headers/real_vector3d.h"

float triple_product3d(const real_vector3d *a, const real_vector3d *b, const real_vector3d *n)
{
    return n->n[0] * (b->n[2] * a->n[1] - b->n[1] * a->n[2])
         + n->n[2] * (a->n[0] * b->n[1] - b->n[0] * a->n[1])
         + n->n[1] * (b->n[0] * a->n[2] - a->n[0] * b->n[2]);
}
