/* scenario_location_from_line @0x83704288 — resolve the BSP location at the end of a line segment.
 * Thunk: only the end point matters, so it defers to scenario_location_from_point. */

#include "headers/location.h"
#include "headers/real_point3d.h"

extern void scenario_location_from_point(location *location, const real_point3d *point);

/* renamed param `location` -> `out_location`: it shadowed the `location` typedef, breaking the
 * later `const location *` parameters in this list. */
void scenario_location_from_line(location *out_location, const location *start_location,
                                 const real_point3d *start_point, const real_point3d *end_point)
{
    scenario_location_from_point(out_location, end_point);
}
