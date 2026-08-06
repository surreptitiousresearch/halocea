/* hcex_is_under_water @0x836E0670 — true when the given world point is underwater. Resolves the BSP
 * location of the point and queries the scenario's current water/wind state there. */

#include <stdint.h>
#include "../headers/location.h"
#include "../headers/real_point3d.h"
#include "../headers/real_vector3d.h"

extern "C" void scenario_location_from_point(location *location, const real_point3d *point);
extern "C" uint8_t scenario_get_current(const location *location, const real_point3d *position, real_vector3d *wind_vector, unsigned int flags);

extern "C" int hcex_is_under_water(float x, float y, float z)
{
    location point_location;
    real_point3d point;
    real_vector3d wind_vector;

    point.n[0] = x;
    point.n[1] = y;
    point.n[2] = z;
    scenario_location_from_point(&point_location, &point);
    return scenario_get_current(&point_location, &point, &wind_vector, 0);
}
