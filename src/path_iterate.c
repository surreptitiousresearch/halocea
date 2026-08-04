/* path_iterate @0x8381DCE8 — one step of path_find's A* search: pop the cheapest step off `path`'s heap, cast
 * a pill from it toward the goal, and either finish (a clean cast reaching the goal's surface becomes the
 * final `goal_step_index`), track the closest blocked-by-the-goal-obstacle candidate, or expand new steps
 * around whatever disc blocked the cast. A cast that stops on a structure edge (edge_index != -1) is dropped
 * without further expansion. Returns whether the search should keep iterating (a step was popped, no goal
 * found yet, and the heap still has entries).
 *
 * DEVIATION — same offset-folding artifact as every other obstacles_* / path_* function this arc: the popped
 * step's base pointer is computed as `(char *)path + 40 * step_index` (48 bytes short of
 * `&path->steps[step_index]`, since `steps` sits at obstacle_path offset 0x30), with the missing +48 folded
 * into every subsequent field-offset constant. Verified field-for-field via disasm_range
 * (0x8381DCFC-0x8381DEB8) against obstacle_path.h's `step` layout.
 *
 * DEVIATION — the DB's own `path_test_pill2d_result` output param is a real struct (distance/surface_index/
 * edge_index/disc_index/obstacle_index), but Hex-Rays split its 5 fields across 4 unrelated-looking locals
 * (`real_point2d v20` for distance+surface_index reinterpreted as two floats, plus separate `v21`/`v22`/`v23`)
 * because they happened to land in one contiguous BYREF stack region — resolved by mapping each back to its
 * real field via the DB's `types_members` layout and offsets confirmed in disasm (var_40/var_3C/var_38/
 * var_34/var_32 relative to the call's outgoing `result` pointer).
 *
 * DEVIATION — `path_test_pill2d`'s `ignore_structure` argument, shown by the decompiler as a raw
 * `_cntlzw(previous_step_index + 1)` call, is actually only the FIRST bit of that count: disasm
 * (0x8381DDA8-0x8381DDB0) shows a following `extrwi r11, r10, 1, 26` that extracts a single bit testing
 * whether the leading-zero count is exactly 32 — i.e. the whole expression reduces to
 * `previous_step_index == -1` (only the very first step in a chain skips the structure test). The
 * `endpoint_is_goal` argument is an unconditional constant `1` (disasm 0x8381DD60/0x8381DD70), not computed
 * at all. `ignore_optional` is `path->ignore_optional` read fresh from the struct (offset 0x2A). */

#include <stdint.h>
#include "headers/obstacle_path.h"
#include "headers/path_test_pill2d_result.h"

#include "headers/structure_bsp.h"
#include "headers/real_vector2d.h"
extern void heap_down(obstacle_path *path, int16_t heap_index);
extern uint8_t path_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *point, int surface_index, const real_vector2d *direction, float radius, float distance, uint8_t ignore_structure, uint8_t endpoint_is_goal, uint8_t ignore_optional, path_test_pill2d_result *result);
extern uint8_t structure_surfaces_are_equivalent(const structure_bsp *structure, const real_point2d *destination_point, int destination_surface_index, int test_surface_index);
extern int16_t path_add_step(obstacle_path *path, const real_point2d *point, int surface_index, int16_t obstacle_index, uint8_t obstacle_direction_index, float previous_distance, int16_t previous_step_index);
extern void path_add_steps(obstacle_path *path, int16_t step_index, int16_t seed_disc_index);

uint8_t path_iterate(obstacle_path *path)
{
    __int16 step_index;

    if (path->heap_count <= 0)
    {
        step_index = -1;
    }
    else
    {
        step_index = path->heap[0];
        path->heap_count--;
        path->heap[0] = path->heap[path->heap_count];
        heap_down(path, 0);
    }

    if (step_index != -1)
    {
        step *current_step = &path->steps[step_index];
        unsigned __int8 ignore_structure = (current_step->previous_step_index == -1);

        path_test_pill2d_result result;
        path_test_pill2d(path->structure, path->ignore_broken_surfaces, path->obstacles, -1,
                &current_step->point, current_step->surface_index, &current_step->direction, path->radius,
                current_step->distance, ignore_structure, 1, path->ignore_optional, &result);

        if (result.edge_index == -1)
        {
            if (result.disc_index == 0xFFFF)
            {
                unsigned __int8 reached_goal;
                if (result.surface_index == path->goal_surface_index)
                    reached_goal = 1;
                else
                    reached_goal = structure_surfaces_are_equivalent(path->structure, &path->goal,
                            path->goal_surface_index, result.surface_index);

                if (reached_goal)
                {
                    real_point2d goal_point;
                    goal_point.n[0] = current_step->direction.n[0] * result.distance + current_step->point.n[0];
                    goal_point.n[1] = current_step->direction.n[1] * result.distance + current_step->point.n[1];

                    float previous_distance =
                            (current_step->total_distance - current_step->distance) + result.distance;
                    path->goal_step_index = path_add_step(path, &goal_point, result.surface_index, -1, 0,
                            previous_distance, step_index);
                }
            }
            else
            {
                if (result.obstacle_index == path->goal_obstacle_index
                        && current_step->distance < path->best_goal_blocked_distance)
                {
                    path->best_goal_blocked_distance = current_step->distance;
                    path->best_goal_blocked_step_index = step_index;
                }
                path_add_steps(path, step_index, result.disc_index);
            }
        }
    }

    if ((unsigned __int16)path->goal_step_index != 0xFFFF)
        return 0;
    return path->heap_count > 0;
}
