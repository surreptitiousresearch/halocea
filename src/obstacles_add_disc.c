/* obstacles_add_disc @0x8382F9AC — appends a new disc to an obstacles working set (capacity 128), projecting
 * the 3D center onto the pathfinding XY plane via the standard projection-axis/sign mapping table (axis=2,
 * sign=1 — always ground-plane projection here). Returns 0 if the set is already full.
 *
 * DEVIATION note: the decompile computes a base pointer `v7 = (char*)obstacles + 20*disc_count` (8 bytes
 * short of the real `&obstacles->discs[disc_count]`) and folds the missing +8 into each field's offset
 * constant instead (writes at v7+8, v7+0xA, v7+0xC, v7+0x10, v7+0x18) — confirmed via disasm_range these
 * resolve to exactly `disc`'s own field offsets (flags@0, obstacle_index@2, object_index@4, center@8,
 * radius@16) plus 8, i.e. genuinely `&obstacles->discs[disc_count]`. Modeled directly via the real struct
 * pointer rather than reproducing the offset-folded intermediate value. */

#include <stdint.h>
#include "headers/obstacles.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


int obstacles_add_disc(obstacles *obstacles, int object_index, int16_t flags,
        const real_point3d *center, float radius)
{
    if (obstacles->disc_count == 128)
        return 0;

    disc *new_disc = &obstacles->discs[obstacles->disc_count];
    obstacles->disc_count++;
    if (flags & 1)
        obstacles->disc_optional_count++;

    new_disc->flags = flags;
    new_disc->object_index = object_index;
    new_disc->obstacle_index = -1;
    new_disc->center.n[0] = center->n[global_projection3d_mappings[2][1][1]];
    new_disc->center.n[1] = center->n[global_projection3d_mappings[2][1][0]];
    new_disc->radius = radius;
    return 1;
}
