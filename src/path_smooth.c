/* path_smooth @0x8381ED38 — collapses a raw parent-chain path (one point per search node) into up to 4
 * smoothed waypoints by cutting corners around obstacles: repeatedly scans forward from the current
 * position for the first run of raw points whose straight line back to the current position is blocked
 * (structure_test_pill2d), then routes around the blocking edge by finding tangent points on both sides
 * of it (find_turning_point cw/ccw, choose_turning_point, find_tangent_point x2, find_avoidance_point),
 * snapping the resulting avoidance point onto the nearest walkable surface
 * (structure_surface_index_from_point + collision_surface_project_point2d) as the next smoothed waypoint.
 * Bails out (steps_finish_path cleared to 0, no final raw-endpoint appended) if a turning point can't be
 * found or the 4-waypoint output buffer fills up; otherwise appends the last raw point once no further
 * obstruction is found and leaves *steps_finish_path untouched (the caller pre-sets it to 1).
 *
 * Clean decompile (no corruption warning), but several call sites had scrambled/mistyped arguments that
 * needed disasm_range verification against the real DB-recovered prototypes of every callee — in every
 * case the callee's actual prototype (from `funcs.prototype`) was trustworthy and the decompiler's own
 * argument reconstruction at the call site was not. Notably: structure_test_pill2d's last two arguments
 * were shown swapped (`(unsigned int)v42` / `(path_collision_result*)1`, i.e. the address of the result
 * scratch buffer and the literal flags value 1 traded places — the real call is flags=1, result=&scratch);
 * both find_turning_point calls had their trailing 4 arguments completely mislabeled by a stale applied
 * prototype (IDA's own inline comments called them "flags"/"result"/etc. in the wrong order — the real
 * roles, confirmed by disasm register tracing, are first_edge_index/clockwise/ignore_broken_surfaces/
 * result). raw_steps[i].point is real_point3d but only x/y are meaningful for the 2D pill test; passed as
 * (const real_point2d*) throughout, matching the disasm's exact register/offset reads. */

#include <stdint.h>
#include "headers/path_state.h"
#include "headers/path_step.h"
#include "headers/path_collision_result.h"
#include "headers/real_point2d.h"
#include "headers/collision_bsp.h"
#include "headers/path_test_pill_endpoint_near_wall_ok_flags.h"

#include "headers/structure_bsp.h"
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/structure_bsp.h"
extern uint8_t structure_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *start_point, int start_surface_index, const real_point2d *end_point, int end_surface_index, float radius, unsigned int flags, path_collision_result *result);
extern uint8_t find_turning_point(const structure_bsp *structure, const real_point2d *p, float radius, int first_edge_index, uint8_t clockwise, uint8_t ignore_broken_surfaces, real_point2d *result);
extern uint8_t choose_turning_point(const real_point2d *start_point, const real_point2d *clockwise_turning_point, const real_point2d *counterclockwise_turning_point, const real_point2d *unobstructed_path_point, const real_point2d *obstructed_path_point, real_point2d *result);
extern void find_tangent_point(const real_point2d *point, const real_point2d *center, float radius, uint8_t clockwise, real_point2d *tangent_point);
extern void find_avoidance_point(const real_point2d *tangent_points, const real_point2d *center, const real_point2d *start_point, float radius, real_point2d *avoidance_point);
extern int structure_surface_index_from_point(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *known_point, int known_surface_index, real_point2d *point);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);

void path_smooth(path_state *state, int16_t raw_step_count, const path_step *raw_steps,
        int16_t *smoothed_step_count, path_step *smoothed_steps, uint8_t *steps_finish_path)
{
    if (raw_step_count <= 1)
    {
        *smoothed_step_count = 1;
        *smoothed_steps = *raw_steps;
        return;
    }

    real_point2d current_position = *(const real_point2d *)&state->input.start_point;
    int current_surface_index = state->input.start_surface_index;
    int16_t smoothed_count = 0;

    int16_t step_index = 1;
    while (1)
    {
        int16_t collision_step_index = -1;
        int collision_edge_index = -1;
        uint8_t collision_active = 0;

        for (int16_t i = step_index; i < raw_step_count; i++)
        {
            path_collision_result collision_result;
            if (structure_test_pill2d(state->structure, state->input.ignore_broken_surfaces, &current_position,
                    current_surface_index, (const real_point2d *)&raw_steps[i].point, raw_steps[i].surface_index,
                    0.30000001f, (1u << _path_test_pill_endpoint_near_wall_ok_bit), &collision_result))
            {
                if (!collision_active)
                {
                    collision_edge_index = collision_result.edge_index;
                    collision_step_index = i;
                    collision_active = 1;
                }
            }
            else if (collision_active)
            {
                collision_step_index = -1;
                collision_edge_index = -1;
                collision_active = 0;
            }
        }

        if (!collision_active || collision_edge_index == -1)
            break;

        real_point2d clockwise_turning_point;
        real_point2d counterclockwise_turning_point;
        uint8_t found_cw = find_turning_point(state->structure, &current_position, 0.30000001f,
                collision_edge_index, 1, state->input.ignore_broken_surfaces, &clockwise_turning_point);
        uint8_t found_ccw = find_turning_point(state->structure, &current_position, 0.30000001f,
                collision_edge_index, 0, state->input.ignore_broken_surfaces, &counterclockwise_turning_point);
        if (!found_ccw || !found_cw)
            goto bail_out;

        const path_step *collision_step = &raw_steps[collision_step_index];
        real_point2d chosen_center;
        uint8_t chose_clockwise = choose_turning_point(&current_position, &clockwise_turning_point,
                &counterclockwise_turning_point, (const real_point2d *)&collision_step[-1].point,
                (const real_point2d *)&collision_step->point, &chosen_center);

        real_point2d tangent_points[2];
        find_tangent_point(&current_position, &chosen_center, 0.34999999f, chose_clockwise, &tangent_points[0]);
        find_tangent_point((const real_point2d *)&collision_step->point, &chosen_center, 0.34999999f,
                !chose_clockwise, &tangent_points[1]);

        real_point2d avoidance_point;
        find_avoidance_point(tangent_points, &chosen_center, &current_position, 0.34999999f, &avoidance_point);

        real_point2d known_point = current_position;
        current_position = avoidance_point;
        current_surface_index = structure_surface_index_from_point(state->structure,
                state->input.ignore_broken_surfaces, &known_point, current_surface_index, &current_position);

        collision_surface_project_point2d((const collision_bsp *)state->structure->collision_bsp.address,
                current_surface_index, 2, 1, &current_position, &smoothed_steps[smoothed_count].point);
        smoothed_steps[smoothed_count].surface_index = current_surface_index;
        smoothed_count++;

        if (smoothed_count >= 4)
            goto bail_out;

        step_index = collision_step_index;
    }

    smoothed_steps[smoothed_count] = raw_steps[raw_step_count - 1];
    smoothed_count++;
    *smoothed_step_count = smoothed_count;
    return;

bail_out:
    *smoothed_step_count = smoothed_count;
    *steps_finish_path = 0;
}
