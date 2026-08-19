/* rasterizer_hud_motion_sensor_blip_end @0x836A6408 — public entry thunk. A single unconditional
 * branch (tail-call) to _rasterizer_hud_motion_sensor_blip_end; forwards all arguments unchanged.
 * Verified against disasm: `b _rasterizer_hud_motion_sensor_blip_end`. */

#include "headers/real_point2d.h"

extern void _rasterizer_hud_motion_sensor_blip_end(const real_point2d *center_point, float theta);

void rasterizer_hud_motion_sensor_blip_end(const real_point2d *center_point, float theta)
{
    _rasterizer_hud_motion_sensor_blip_end(center_point, theta);
}
