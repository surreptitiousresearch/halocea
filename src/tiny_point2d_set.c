#include "headers/motion_sensor_globals.h"
#include "headers/hud_globals.h"

void tiny_point2d_set(tiny_point2d *tiny_position, real_point2d *position)
{
    tiny_position->x = (int)((position->n[0] / hud_globals->defaults.motion_sensor_range) * 127.0f);
    tiny_position->y = (int)((position->n[1] / hud_globals->defaults.motion_sensor_range) * 127.0f);
}
