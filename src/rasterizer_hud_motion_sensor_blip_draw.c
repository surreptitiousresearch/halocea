/* rasterizer_hud_motion_sensor_blip_draw @0x836A6400 — public entry thunk. A single unconditional
 * branch (tail-call) to _rasterizer_hud_motion_sensor_blip_draw; forwards all arguments unchanged.
 * Verified against disasm: `b _rasterizer_hud_motion_sensor_blip_draw` (image word 0x48103F40,
 * AA=0/LK=0, LI=+0x103F40 -> 0x837AA340); funcs.size = 4. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"

extern void _rasterizer_hud_motion_sensor_blip_draw(const real_point2d *blip_position, float fade, float radius, const real_rgb_color *blip_color, uint8_t custom);

void rasterizer_hud_motion_sensor_blip_draw(const real_point2d *blip_position, float fade, float radius,
                                            const real_rgb_color *blip_color, uint8_t custom)
{
    _rasterizer_hud_motion_sensor_blip_draw(blip_position, fade, radius, blip_color, custom);
}
