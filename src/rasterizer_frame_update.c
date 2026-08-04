/* rasterizer_frame_update @0x837241B8 — stashes this frame's delta time into the global rasterizer
 * frame parameters (the slot two floats past game_time_sec). */

#include "headers/global_frame_parameters.h"

/* param attested float: sole use is stfs (single-precision store), Blam `real dt` convention */
void rasterizer_frame_update(float dt)
{
    *((float *)&global_frame_parameters.game_time_sec + 2) = dt;
}
