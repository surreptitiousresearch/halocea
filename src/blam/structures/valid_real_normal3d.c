/* valid_real_normal3d @0x8370F660 — true when a vector is a unit normal: its squared length is within 0.001
 * of 1.0 and not NaN. (The decompiler's trailing long-double parameter is an FPR-shadow phantom.) */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern double __fabs(double x);
extern int _isnan(double x);

uint8_t valid_real_normal3d(const real_vector3d *n)
{
    float deviation =
        (n->n[2] * n->n[2] + (n->n[0] * n->n[0] + n->n[1] * n->n[1])) - 1.0f;
    if (_isnan(deviation))
        return 0;
    /* epsilon is the float constant 0x3A83126F (0.001f), disasm 8370F6AC lfs */
    if (__fabs(deviation) >= 0.001f)
        return 0;
    return 1;
}
