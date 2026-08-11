/* scenario_ensure_point_within_world @0x83704290 — nudge a point upward in 5cm steps until it lies inside
 * the world BSP (or after 150 attempts), so a re-placed object isn't left embedded in/below geometry.
 * Returns true when the point was already inside (no adjustment needed). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/bsp3d.h"
#include "headers/blam_data_globals.h"

extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);

int scenario_ensure_point_within_world(real_point3d *point)
{
    int16_t attempts = 0;
    for (; bsp3d_test_point(global_bsp3d, 0, point) == -1; point->n[2] = point->n[2] + 0.050000001f)
    {
        if (attempts++ >= 150)
            break;
    }
    return attempts == 0;
}
