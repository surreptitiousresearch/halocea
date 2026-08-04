/* rasterizer_hud_motion_sensor_blip_begin @0x836A63F8 — public entry thunk. A single unconditional
 * branch (tail-call) to _rasterizer_hud_motion_sensor_blip_begin; takes and forwards no arguments.
 * Verified against disasm: `b _rasterizer_hud_motion_sensor_blip_begin`. */

extern void _rasterizer_hud_motion_sensor_blip_begin(void);

void rasterizer_hud_motion_sensor_blip_begin(void)
{
    _rasterizer_hud_motion_sensor_blip_begin();
}
