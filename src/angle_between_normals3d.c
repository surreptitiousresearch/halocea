/* angle_between_normals3d @0x836FC3E8 — angle (radians) between two unit vectors: acos of their clamped
 * dot product. If the two vectors are bit-for-bit identical the dot/acos is skipped and 0 is returned.
 *
 * DEVIATION: the decompiler invented a spurious trailing `long double a3` parameter — it is actually the
 * function-local double slot used to hold the clamped dot before the acos call. The disassembly (r3, r4
 * only; f-args untouched at entry) confirms the real prototype takes just the two vector pointers. */

#include <math.h>
#include "headers/real_vector3d.h"

float angle_between_normals3d(const real_vector3d *a, const real_vector3d *b)
{
    /* Bit-identical normals → zero angle (matches the shipped integer bit compare of the three components). */
    if ( *(const int *)&a->n[0] == *(const int *)&b->n[0]
      && *(const int *)&a->n[1] == *(const int *)&b->n[1]
      && *(const int *)&a->n[2] == *(const int *)&b->n[2] )
        return 0.0f;

    float dot = (a->n[0] * b->n[0]) + ((a->n[2] * b->n[2]) + (a->n[1] * b->n[1]));
    if ( dot < -1.0f )
        dot = -1.0f;
    else if ( dot > 1.0f )
        dot = 1.0f;

    return (float)acos(dot);
}
