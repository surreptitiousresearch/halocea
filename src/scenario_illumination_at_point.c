/* scenario_illumination_at_point @0x83703594 — stub environment-lighting query: always reports straight-up
 * ambient lighting (surface normal = world up, radiosity direction = world left, radiosity/diffuse color =
 * flat white), for any of the output pointers the caller actually wants. Always succeeds. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


uint8_t scenario_illumination_at_point(const real_point3d *point, real_vector3d *surface_normal, real_vector3d *radiosity_vector, real_rgb_color *radiosity_color, real_rgb_color *diffuse_color)
{
    if ( surface_normal )
        *surface_normal = *global_up3d;
    if ( radiosity_vector )
        *radiosity_vector = *global_left3d;
    if ( radiosity_color )
        *radiosity_color = *global_real_rgb_white;
    if ( diffuse_color )
        *diffuse_color = *global_real_rgb_white;
    return 1;
}
