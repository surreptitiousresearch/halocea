/* points_within_distance @0x837C5150 — scans `points` in order for the first one whose signed distance
 * along the render camera's forward vector (from the camera position) does not exceed `distance`.
 *
 * DEVIATION: despite the name, this is not an "all points within distance" test — it returns true as soon
 * as it finds ONE point within distance, and only returns false once every point has been scanned and all
 * of them exceeded it (or there are no points at all). Preserved exactly as the original's while-loop
 * control flow dictates rather than assuming an all-points test. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


uint8_t points_within_distance(int16_t point_count, const real_point3d *points, float distance)
{
    if ( point_count <= 0 )
        return 0;

    __int16 i = 0;
    while ( render.camera.forward.n[1] * (points[i].n[1] - render.camera.position.n[1])
          + render.camera.forward.n[2] * (points[i].n[2] - render.camera.position.n[2])
          + render.camera.forward.n[0] * (points[i].n[0] - render.camera.position.n[0]) > distance )
    {
        ++i;
        if ( i >= point_count )
            return 0;
    }

    return 1;
}
