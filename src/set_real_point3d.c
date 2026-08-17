/* set_real_point3d @0x8368C7F0 */
#include "headers/real_point3d.h"

/* Params are Blam `real` (float); the Hex-Rays PPC decompiler defaulted them to `double`.
 * The binary stores each straight through with stfs (no frsp narrowing) -> genuine float. */
real_point3d *set_real_point3d(real_point3d *p, float x, float y, float z)
{
    p->n[0] = x;
    p->n[1] = y;
    p->n[2] = z;
    return p;
}
