/* path_add_steps @0x8381DA40 — flood-fills the obstacle-disc cluster starting at `seed_disc_index` (same BFS
 * pattern as obstacles_recompute) and, for every disc reachable within that cluster, computes its two tangent
 * lines from `path->steps[step_index]` and tries casting a pill from the step's point along each tangent to a
 * point midway past the disc. If that midpoint pill lands closer than the tangent point's own clearance
 * distance and doesn't just re-collide with the SAME obstacle the step is already routing around, a new step
 * is added there (`path_add_step`) heading further out from the disc, using the surface found by a bare ray
 * cast to the midpoint (its result otherwise discarded, matching the "probe" pattern used inside
 * path_test_pill2d itself). Newly-discovered discs (via path_test_pill2d's own disc hit, when that hit isn't
 * already in the visited set) are enqueued to also be processed by this same flood fill.
 *
 * DEVIATION — reconstructed from disasm_range (0x8381DA40-0x8381DCD8); the DB's own decompile has heavy
 * register reuse (the same locals renamed across unrelated purposes at different points, e.g. `v8`/`v12`) and
 * mistypes `obstacles_disc_tangents`'s two direction outputs as an `(unsigned __int8)` cast and a stray
 * `float[2]`, and fabricates the trailing `path_test_pill2d` call args (`v23`/`v24`/`v25` are all read
 * uninitialized). Register evidence shows: `obstacles_disc_tangents`'s `right_direction`/`left_direction`
 * outputs land in one contiguous 2-element `real_vector2d` array (var_1C0 immediately followed by var_1B8,
 * 8 bytes apart) reproduced here as `tangent_directions[2]`; the `path_test_pill2d` call's `ignore_structure`/
 * `endpoint_is_goal` args are both literal 0, and `ignore_optional` is `path->ignore_optional`; and the
 * `structure_test_ray2d` call right before `path_add_step` discards its hit boolean/distance/edge_index but
 * (like path_test_pill2d's own probe rays) its `result.surface_index` field is read back and used as
 * `path_add_step`'s `surface_index` argument. */

#include <stdint.h>
#include <string.h>
#include "headers/bit_vector.h"
#include "headers/obstacle_path.h"
#include "headers/path_test_pill2d_result.h"
#include "headers/structure_test_ray2d_result.h"
#include "headers/structure_bsp.h"
#include "headers/structure_bsp.h"

extern void obstacles_disc_tangents(const obstacles *obstacles, int16_t disc_index, const real_point2d *point, float radius, real_vector2d *right_direction, real_vector2d *left_direction, float *tangent_distance);
extern uint8_t path_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *point, int surface_index, const real_vector2d *direction, float radius, float distance, uint8_t ignore_structure, uint8_t endpoint_is_goal, uint8_t ignore_optional, path_test_pill2d_result *result);
extern uint8_t structure_test_ray2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *point, int surface_index, const real_vector2d *direction, float distance, structure_test_ray2d_result *result);
extern int16_t path_add_step(obstacle_path *path, const real_point2d *point, int surface_index, int16_t obstacle_index, uint8_t obstacle_direction_index, float previous_distance, int16_t previous_step_index);

void path_add_steps(obstacle_path *path, int16_t step_index, int16_t seed_disc_index)
{
    step *current_step = &path->steps[step_index];

    unsigned int visited[4];
    memset(visited, 0, 4 * BIT_VECTOR_SIZE_IN_LONGS(path->obstacles->disc_count));

    int16_t queue[132];
    int queue_count = 1;
    queue[0] = seed_disc_index;
    BIT_VECTOR_SET_FLAG(visited, seed_disc_index);

    do
    {
        int16_t disc_index = queue[--queue_count];

        int16_t obstacle_index = (disc_index == -1) ? -1 : path->obstacles->discs[disc_index].obstacle_index;

        real_vector2d tangent_directions[2];
        float tangent_distance;
        obstacles_disc_tangents(path->obstacles, disc_index, &current_step->point, path->radius,
                &tangent_directions[0], &tangent_directions[1], &tangent_distance);

        if (tangent_distance < path->radius)
            tangent_distance = path->radius;

        for (int side = 0; side < 2; side++)
        {
            const real_vector2d *direction = &tangent_directions[side];

            path_test_pill2d_result pill_hit;
            path_test_pill2d(path->structure, path->ignore_broken_surfaces, path->obstacles, disc_index,
                    &current_step->point, current_step->surface_index, direction, path->radius,
                    path->radius * 2.0f + tangent_distance, 0, 0, path->ignore_optional, &pill_hit);

            if ((uint16_t)pill_hit.disc_index != 0xFFFF)
            {
                int16_t hit_disc_index = pill_hit.disc_index;
                int word_index = hit_disc_index >> 5;
                unsigned int bit = 1 << (hit_disc_index & 0x1F);
                if ((visited[word_index] & bit) == 0)
                {
                    visited[word_index] |= bit;
                    queue[queue_count++] = hit_disc_index;
                }
            }

            if (pill_hit.distance > tangent_distance && pill_hit.obstacle_index != obstacle_index)
            {
                float midpoint_distance = (pill_hit.distance + tangent_distance) * 0.5f;

                structure_test_ray2d_result midpoint_probe; /* only midpoint_probe.surface_index is used */
                structure_test_ray2d(path->structure, path->ignore_broken_surfaces, &current_step->point,
                        current_step->surface_index, direction, midpoint_distance, &midpoint_probe);

                real_point2d midpoint;
                midpoint.n[0] = direction->n[0] * midpoint_distance + current_step->point.n[0];
                midpoint.n[1] = direction->n[1] * midpoint_distance + current_step->point.n[1];

                float previous_distance = (current_step->total_distance - current_step->distance) + midpoint_distance;
                path_add_step(path, &midpoint, midpoint_probe.surface_index, obstacle_index, side,
                        previous_distance, step_index);
            }
        }
    }
    while (queue_count > 0);
}
