/* point_from_line2d @0x836FBDF8 */
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

/* result = point + direction * t.
 * DEVIATION: decompiler split the r6 result pointer into a phantom `result`
 * (reserved GPR of the float `t`) plus `a5`; disassembly confirms r6 is the
 * single result pointer and f1 carries `t`. */
real_point2d * point_from_line2d(const real_point2d *point, const real_vector2d *direction, float t, real_point2d *result)
{
    result->n[0] = direction->n[0] * t + point->n[0];
    result->n[1] = direction->n[1] * t + point->n[1];
    return result;
}
