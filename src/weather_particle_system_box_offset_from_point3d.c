#include "headers/real_point3d.h"

extern void weather_particle_system_wrap_point(float box_width, const real_point3d *point, real_point3d *result);

void weather_particle_system_box_offset_from_point3d(float box_width, const real_point3d *point, real_point3d *result, float *offset)
{
    weather_particle_system_wrap_point(box_width, point, result);

    offset[0] = result->n[0] - offset[0];
    offset[1] = result->n[1] - offset[1];
    offset[2] = result->n[2] - offset[2];
}
