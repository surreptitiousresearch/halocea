/* compare_targets @0x836C5BD8 — qsort comparator ranking aim-assist targets: higher autoaim then
 * higher magnetism first, then nearer distance, then smaller angle, finally lower object index. */

#include "headers/aim_assist_target.h"

int compare_targets(const aim_assist_target *target0, const aim_assist_target *target1)
{
    if (target0->autoaim_level > target1->autoaim_level) return -1;
    if (target0->autoaim_level < target1->autoaim_level) return 1;

    if (target0->magnetism_level > target1->magnetism_level) return -1;
    if (target0->magnetism_level < target1->magnetism_level) return 1;

    if (target0->distance < target1->distance) return -1;
    if (target0->distance > target1->distance) return 1;

    if (target0->angle < target1->angle) return -1;
    if (target0->angle > target1->angle) return 1;

    return (unsigned __int16)target0->object_index - (unsigned __int16)target1->object_index;
}
