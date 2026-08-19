/* set_real_vector3d @0x8368C800 */
#include "headers/real_vector3d.h"

/* Params are Blam `real` (float); decompiler defaulted them to `double` (stored via stfs, no frsp). */
real_vector3d *set_real_vector3d(real_vector3d *v, float i, float j, float k)
{
    v->n[0] = i;
    v->n[1] = j;
    v->n[2] = k;
    return v;
}
