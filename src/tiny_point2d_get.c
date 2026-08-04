/* tiny_point2d_get @0x837BC6F0 — DEVIATION: the decompiler flagged "local variable allocation has failed"
 * and fabricated two extra parameters (a3, a4). disasm_range(0x837BC6F0,0x837BC754) confirms the DB's own
 * 2-arg funcs.prototype: only tiny_position and position are ever touched. */

#include "headers/motion_sensor_globals.h"
#include "headers/hud_globals.h"

void tiny_point2d_get(tiny_point2d *tiny_position, real_point2d *position)
{
    position->n[0] = (float)tiny_position->x * hud_globals->defaults.motion_sensor_range * 0.0078740157f;
    position->n[1] = (float)tiny_position->y * hud_globals->defaults.motion_sensor_range * 0.0078740157f;
}
