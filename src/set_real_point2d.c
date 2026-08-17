/* set_real_point2d @0x836FBDE8 */
#include "headers/real_point2d.h"

/* Params are Blam `real` (float); decompiler defaulted them to `double` (stored via stfs, no frsp). */
real_point2d *set_real_point2d(real_point2d *p, float x, float y)
{
    p->n[0] = x;
    p->n[1] = y;
    return p;
}
