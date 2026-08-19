/* blip_begin @0x837BD6F0 — sets up global motion-sensor blip render state (owning player index, blip
 * scale, and center point) before rasterizer_hud_motion_sensor_blip_begin draws the blip.
 *
 * DEVIATION: the decompiler invents a 4th "num_blips" __QWORD* param and renders the center-point store as
 * an indirect `*(_QWORD*)(*(_DWORD*)num_blips - 13960) = *num_blips`. Disasm shows only 3 registers used
 * (r3, r4, r5) and the store target is the fixed global `center_point`, not a computed pointer — reproduced
 * here per disasm as a 3-param function with a direct global assignment. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/blam_data_globals.h"


extern void rasterizer_hud_motion_sensor_blip_begin(void);

void blip_begin(const real_point2d *center, uint8_t in_multiplayer, int16_t local_player_index)
{
    blip_player_index = local_player_index;
    scale_2 = in_multiplayer ? 0.75f : 1.0f;
    center_point = *center;
    rasterizer_hud_motion_sensor_blip_begin();
}
