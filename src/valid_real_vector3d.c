/* valid_real_vector3d @0x8379A698 — DEVIATION: same bogus `long double a2` 2nd-parameter artifact as
 * valid_real_point3d.c; the DB's own funcs.prototype confirms the true signature takes only `v`. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern int _isnan(double x);

uint8_t valid_real_vector3d(const real_vector3d *v)
{
    if ( _isnan(v->n[0]) )
        return 0;
    if ( _isnan(v->n[1]) )
        return 0;
    if ( _isnan(v->n[2]) )
        return 0;
    return 1;
}
