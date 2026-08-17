/* point_from_line3d @0x8368C810 */
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

/* result = point + direction * t.
 * DEVIATION: decompiler split the r6 result pointer into a phantom `result`
 * (reserved GPR of the float `t`) plus `a5`; the single result pointer is r6. */
real_point3d * point_from_line3d(const real_point3d *point, const real_vector3d *direction, float t, real_point3d *result)
{
    result->n[0] = direction->n[0] * t + point->n[0];
    result->n[1] = direction->n[1] * t + point->n[1];
    result->n[2] = direction->n[2] * t + point->n[2];
    return result;
}
