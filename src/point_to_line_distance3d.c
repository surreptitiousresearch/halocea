/* point_to_line_distance3d @0x837EE628 */
#include <math.h>

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);

float point_to_line_distance3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height)
{
    return sqrtf(point_to_line_distance_squared3d(point, base, height));
}
