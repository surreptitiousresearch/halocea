/* path_add_step @0x8381D3C8 — appends a new step to `path` heading from `point` toward the goal (direction =
 * goal - point, then normalized; distance = that length, total_distance = distance + previous_distance), then
 * pushes it onto the min-heap (by total_distance) for path_iterate to pop. Returns -1 if `path->steps` is
 * already full (128), otherwise the new step's index.
 *
 * Before inserting, when `previous_step_index` traces an unbroken chain of steps all wrapping the SAME
 * `obstacle_index` back to its root, and that obstacle is the goal's own blocking obstacle
 * (`path->goal_obstacle_index`), the new step is checked against the chain root's already-recorded
 * "obstructed toward the goal" step on the OPPOSITE wrap direction: if that opposite-direction chain's own
 * direction vector already points at the goal in a way that would cross this new step's approach, the new
 * step is suppressed entirely (self-crossing wrap around the same obstacle from both sides is redundant).
 * Otherwise the chain root's same-direction "obstructed toward goal" slot is updated to point at the new step
 * whenever it was empty or previously pointed at `previous_step_index` itself.
 *
 * DEVIATION — the DB's own prototype has 7 parameters (confirmed via `funcs.prototype`); Hex-Rays additionally
 * fabricates a phantom 8th (`a8`) from the same dead-float-shadow-GPR pattern as every other function on this
 * call path (`previous_distance`, param 6, is a float and consumes a dead shadow at r8). Disasm_range
 * (0x8381D3C8-0x8381D440) shows the REAL `previous_step_index` lands in r9 and is copied to r25, which is
 * exactly the register Hex-Rays' phantom `a8` reads from — while the decompiler's own declared param 7
 * (also named `previous_step_index`, bound to whatever register precedes the shadow) is never referenced
 * anywhere in the body. `a8` is `previous_step_index` throughout below; the decompile's dead param 7 binding
 * is discarded.
 *
 * DEVIATION — same offset-folding artifact as the rest of this arc: the new step's base pointer is computed
 * as `(char *)path + 40 * step_count` (48 bytes short of `&path->steps[step_count]`), and the chain-walk /
 * obstructed-slot lookups similarly compute `(char *)path + 40 * index` or `&path->radius + 10 * index`
 * (missing the same +48). All folded field-offset constants were verified against `step`'s real layout via
 * disasm_range and reconstructed with direct `path->steps[index]` indexing. The final heap push's raw
 * `&path->radius + 2*(index + 2585)` pointer arithmetic is likewise just `&path->heap[index]` (heap sits at
 * obstacle_path offset 0x1432 = 5170 bytes; 2*index + 5170 in bytes == 2*(index + 2585)) — reconstructed as a
 * standard min-heap sift-up over `path->heap[]`/`path->steps[].total_distance` rather than reproducing the
 * pointer arithmetic. */

#include <stdint.h>
#include "headers/obstacle_path.h"

extern float normalize2d(real_vector2d *v);

int16_t path_add_step(obstacle_path *path, const real_point2d *point, int surface_index, int16_t obstacle_index,
        uint8_t obstacle_direction_index, float previous_distance, int16_t previous_step_index)
{
    if (path->step_count >= 128)
        return -1;

    uint8_t blocks_goal = 0;
    uint8_t allow_step = 1;

    float goal_delta_y = path->goal.n[1] - point->n[1];

    if (previous_step_index != -1)
    {
        int16_t chain_index = previous_step_index;
        step *chain_step;

        for (;;)
        {
            chain_step = &path->steps[chain_index];
            if (chain_step->obstacle_index != obstacle_index)
                break;
            if (chain_step->obstacle_direction_index != obstacle_direction_index)
            {
                allow_step = 0;
                goto chain_walk_done;
            }
            chain_index = chain_step->previous_step_index;
            if (chain_index == -1)
                goto chain_walk_done;
        }

        if (obstacle_index == path->goal_obstacle_index && path->goal_obstacle_index != -1)
        {
            blocks_goal = 1;

            int16_t opposite_obstructed_index =
                    chain_step->obstructed_goal_step_indices[obstacle_direction_index == 0];
            if (opposite_obstructed_index != -1)
            {
                const step *opposite_step = &path->steps[opposite_obstructed_index];
                const step *previous_step = &path->steps[previous_step_index];
                float goal_delta_x = path->goal.n[0] - point->n[0];

                float dot = opposite_step->direction.n[0] * goal_delta_x
                        + opposite_step->direction.n[1] * goal_delta_y;
                float cross_check = (previous_step->direction.n[1] * opposite_step->direction.n[0]
                                - opposite_step->direction.n[1] * previous_step->direction.n[0])
                        * (opposite_step->direction.n[0] * goal_delta_y
                                - opposite_step->direction.n[1] * goal_delta_x);

                if (dot > 0.0f && cross_check < 0.0f)
                    allow_step = 0;
            }

            if (allow_step)
            {
                int16_t *same_direction_slot =
                        &chain_step->obstructed_goal_step_indices[obstacle_direction_index];
                if (*same_direction_slot == previous_step_index || *same_direction_slot == -1)
                    *same_direction_slot = path->step_count;
            }
        }
    }

chain_walk_done:
    if (!allow_step)
        return -1;

    int16_t new_step_index = path->step_count;
    path->step_count++;

    step *new_step = &path->steps[new_step_index];
    new_step->point = *point;
    new_step->surface_index = surface_index;
    new_step->direction.n[0] = path->goal.n[0] - point->n[0];
    new_step->direction.n[1] = goal_delta_y;

    float distance = normalize2d(&new_step->direction);
    new_step->distance = distance;
    new_step->obstacle_index = obstacle_index;
    new_step->total_distance = distance + previous_distance;
    new_step->obstacle_direction_index = obstacle_direction_index;
    new_step->previous_step_index = previous_step_index;
    new_step->obstructed_goal_step_indices[0] = -1;
    new_step->obstructed_goal_step_indices[1] = -1;

    if (blocks_goal && new_step->distance < path->best_goal_blocked_distance)
    {
        path->best_goal_blocked_distance = new_step->distance;
        path->best_goal_blocked_step_index = new_step_index;
    }

    if (path->heap_count < 128)
    {
        int16_t child_index = path->heap_count;
        path->heap_count++;
        path->heap[child_index] = new_step_index;

        while (child_index > 0)
        {
            int16_t child_heap_slot = path->heap[child_index];
            int16_t parent_index = (child_index - 1) >> 1;
            int16_t parent_heap_slot = path->heap[parent_index];

            if (path->steps[parent_heap_slot].total_distance <= path->steps[child_heap_slot].total_distance)
                break;

            path->heap[parent_index] = child_heap_slot;
            path->heap[child_index] = parent_heap_slot;
            child_index = parent_index;
        }
    }

    return new_step_index;
}
