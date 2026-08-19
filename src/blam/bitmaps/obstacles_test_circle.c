/* obstacles_test_circle @0x8382FDD8 — linear scan for the first disc (other than `ignore_disc_index`) whose
 * radius-padded circle contains `center`; returns that disc's index, or -1 if none match (including the
 * empty-set case). Used by path_find to seed `path->goal_obstacle_index` for the goal point.
 *
 * DEVIATION — same offset-folding decompiler artifact as obstacles_add_disc/obstacles_recompute: the disc
 * base pointer is computed as `(float *)(&obstacles->obstacle_count + 10 * index)` (8 bytes short of
 * `&obstacles->discs[index]`), with the missing +8 folded into the float-index field offsets (4/5/6 =
 * center.x/center.y/radius). Reconstructed with direct `disc *` pointers instead. */

#include <stdint.h>
#include "headers/obstacles.h"
#include "headers/real_point2d.h"

int16_t obstacles_test_circle(const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *center,
        float radius)
{
    for (int16_t disc_index = 0; disc_index < obstacles->disc_count; disc_index++)
    {
        if (disc_index == ignore_disc_index)
            continue;

        const disc *candidate = &obstacles->discs[disc_index];
        float dx = candidate->center.n[0] - center->n[0];
        float dy = candidate->center.n[1] - center->n[1];
        float combined_radius = candidate->radius + radius;

        if (dx * dx + dy * dy <= combined_radius * combined_radius)
            return disc_index;
    }

    return -1;
}
