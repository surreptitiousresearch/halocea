/* ai_generate_line_of_fire_pill @0x836E7E8C — build one line-of-fire pill from a unit's physics capsule.
 * biped_get_physics_pill returns the capsule base point, its height, and its radius. A zero height makes
 * the pill a sphere; otherwise it is a vertical capsule whose directed height is (0,0,height). The pill's
 * width is the capsule radius padded by 0.15 world units, and it is tagged with the prop and unit it
 * represents. */

#include "headers/line_of_fire_pill.h"
#include "headers/real_point3d.h"

extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);

void ai_generate_line_of_fire_pill(int unit_index, int prop_index, line_of_fire_pill *pill)
{
    float height;
    float radius;
    biped_get_physics_pill(unit_index, &pill->base, &height, &radius);

    pill->spherical = (height == 0.0f);
    pill->directed_height.n[0] = 0.0f;
    pill->directed_height.n[1] = 0.0f;
    pill->directed_height.n[2] = height;
    pill->prop_index = prop_index;
    pill->unit_index = unit_index;
    pill->width = radius + 0.15000001f;
}
