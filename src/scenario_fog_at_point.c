/* scenario_fog_at_point @0x83703580 — fog density along a view ray. This build is stubbed to always report no
 * fog (0.0).
 *
 * Deviation: the decompiler's return of *((float*)&v3+1) is the usual single/double register-aliasing
 * artifact; the value returned is 0.0f. */

#include "headers/location.h"
#include "headers/real_point3d.h"

float scenario_fog_at_point(const location *viewer_location, const real_point3d *viewer_point,
                            const real_point3d *point)
{
    return 0.0f;
}
