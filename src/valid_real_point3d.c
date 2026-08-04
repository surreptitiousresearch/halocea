/* valid_real_point3d @0x837FCFB8 — DEVIATION: the decompiler invented a bogus `long double a2` 2nd
 * parameter (an artifact of tracking leftover FPR state across the _isnan calls); the DB's own funcs.prototype
 * confirms the true signature takes only `p`. */

#include "headers/real_point3d.h"

extern int _isnan(double x);

unsigned __int8 valid_real_point3d(const real_point3d *p)
{
    if ( _isnan(p->n[0]) )
        return 0;
    if ( _isnan(p->n[1]) )
        return 0;
    if ( _isnan(p->n[2]) )
        return 0;
    return 1;
}
