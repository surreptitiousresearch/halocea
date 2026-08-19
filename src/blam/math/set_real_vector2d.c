/* set_real_vector2d @0x836C69B8 */
#include "headers/real_vector2d.h"

/* Params are Blam `real` (float); decompiler defaulted them to `double` (stored via stfs, no frsp). */
real_vector2d *set_real_vector2d(real_vector2d *v, float i, float j)
{
    v->n[0] = i;
    v->n[1] = j;
    return v;
}
