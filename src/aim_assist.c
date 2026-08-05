/* aim_assist @0x836C6398 — finds the best aim-assist target for a shot from position along direction.
 * Gathers candidate targets in the shooter's cluster, sorts them by priority, and returns the first
 * one with a clear line of sight. Returns 1 and fills *target on success, 0 if none qualifies. */

#include <stdint.h>
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern int16_t scenario_cluster_index_from_point(const real_point3d *point);
/* def (src/find_aim_assist_targets.c) types arg5 `aim_assist_target *` (an object index carried as a
 * pointer-typed slot; the def casts it back to int internally); matched here. */
extern int16_t find_aim_assist_targets(const aim_assist_parameters *parameters, int16_t position_cluster_index, const real_point3d *position, const real_vector3d *direction, aim_assist_target *ignore_object_index, int16_t ignore_team_index, int16_t maximum_target_count, aim_assist_target *targets);
extern int compare_targets(const aim_assist_target *target0, const aim_assist_target *target1);
extern uint8_t aim_assist_clear_line_of_sight(const real_point3d *p0, const real_point3d *p1, int ignore_object_index, int target_object_index);
extern void  qsort(void *base, unsigned int count, unsigned int size,
                   int (__fastcall *compare)(const void *, const void *));
extern void *memcpy(void *dst, const void *src, unsigned int size);

uint8_t aim_assist(const aim_assist_parameters *parameters, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, int16_t ignore_team_index, aim_assist_target *target)
{
    aim_assist_target targets[64];

    int cluster_index = scenario_cluster_index_from_point(position);
    if (cluster_index == -1)
        return 0;

    int count = find_aim_assist_targets(parameters, cluster_index, position, direction,
                                        (aim_assist_target *)ignore_object_index, ignore_team_index, 64, targets);
    if (count <= 0)
        return 0;

    qsort(targets, count, sizeof(aim_assist_target),
          (int (__fastcall *)(const void *, const void *))compare_targets);

    int i = 0;
    while (!aim_assist_clear_line_of_sight(position, &targets[i].position,
                                           ignore_object_index, targets[i].object_index))
    {
        i = (int16_t)(i + 1);
        if (i >= count)
            return 0;
    }

    memcpy(target, &targets[i], sizeof(aim_assist_target));
    return 1;
}
