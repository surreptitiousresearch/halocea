#include <stdint.h>
#include "headers/real_point3d.h"

extern void light_compute_bounding_sphere(int light_index, uint8_t maximum, uint8_t specular, uint8_t lens_flare_only, real_point3d *bounding_sphere_center, float *bounding_sphere_radius);

void light_get_bounding_sphere(int light_index, real_point3d *position, float *radius)
{
    light_compute_bounding_sphere(light_index, 1u, 0, 1u, position, radius);
}
