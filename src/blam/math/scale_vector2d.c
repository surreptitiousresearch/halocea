/* scale_vector2d @0x8369DC30 */
#include "headers/real_vector2d.h"

/* DEVIATION: decompiler confused signature; disasm: r3=v, f1=c, r5=result */
real_vector2d * scale_vector2d(const real_vector2d *v, float c, real_vector2d *result)
{
    result->n[0] = v->n[0] * c;
    result->n[1] = v->n[1] * c;
    return result;
}
