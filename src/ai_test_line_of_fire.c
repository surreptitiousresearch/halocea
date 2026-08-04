/* ai_test_line_of_fire @0x836E80F4 — tests whether a shot from origin along vector would pass any
 * friendly unit's collision pill. Returns 1 when the line of fire is clear, 0 when a friend blocks it;
 * the blocking friend's prop index is written through prop_index_reference (when supplied). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/line_of_fire_pill.h"

extern __int16 ai_find_line_of_fire_friend_pills(int actor_index, __int16 max_pill_count, line_of_fire_pill *pills);
extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern uint8_t vector_intersects_pill3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *base, const real_vector3d *height, float width);

uint8_t ai_test_line_of_fire(
    int actor_index,
    int ignore_unit_index,
    const real_point3d *origin,
    const real_vector3d *vector,
    int *prop_index_reference)
{
    int clear = 1;
    int prop_index = -1;
    line_of_fire_pill pills[32];

    int pill_count = ai_find_line_of_fire_friend_pills(actor_index, 32, pills);
    if (pill_count > 0)
    {
        int i = 0;
        int blocker = 0;
        while (1)
        {
            blocker = i;
            if (pills[i].unit_index != ignore_unit_index)
            {
                int hit = pills[blocker].spherical
                    ? fast_vector_intersects_sphere(origin, vector, &pills[blocker].base, pills[blocker].width)
                    : vector_intersects_pill3d(origin, vector, &pills[blocker].base,
                                               &pills[blocker].directed_height, pills[blocker].width);
                if (hit)
                    break;
            }
            i = (__int16)(i + 1);
            if (i >= pill_count)
                goto done;
        }
        clear = 0;
        prop_index = pills[blocker].prop_index;
    }

done:
    if (prop_index_reference)
        *prop_index_reference = prop_index;
    return clear;
}
