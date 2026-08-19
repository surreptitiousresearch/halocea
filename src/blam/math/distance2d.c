/* distance2d @0x837C70C0 */
#include <math.h>

#include "headers/real_point2d.h"

float distance2d(const real_point2d *a, const real_point2d *b)
{
    float dx = b->n[0] - a->n[0];
    float dy = b->n[1] - a->n[1];
    return sqrtf(dx * dx + dy * dy);
}
