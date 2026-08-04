/* path_avoid_obstacles @0x8381E008 — final pathfinding pass: walks the smoothed step list produced by
 * path_smooth and re-solves each segment through path_find's obstacle-aware A* (obstacles gathered via
 * obstacles_get_discs_in_sphere/obstacles_add_disc/obstacles_recompute), reconstructing up to 4 avoided
 * waypoints total. Each segment's path_find result is walked backwards through its own internal step
 * chain (obstacle_path.steps[], linked via previous_step_index) and the waypoints are projected back onto
 * the collision BSP surface via collision_surface_project_point2d before being appended to avoided_steps.
 *
 * DEVIATION — two multi-argument call sites were scrambled by the decompiler (the established pattern
 * from path_smooth: dead float-shadow GPR slots and 2-slot stack spills throw off Hex-Rays' positional
 * argument labels even though the enclosing function shows no corruption warning). Both were rebuilt from
 * disasm_range ground truth cross-checked against funcs.prototype:
 *   - obstacles_get_discs_in_sphere: decompile showed a stray uninitialized local ("v30") for
 *     movement_direction and `(int)&v52` for ignore_source_object_index; disasm (0x8381E1A8-0x8381E228)
 *     shows r6=&direction (the just-computed normalized delta), r7=state->input.ignore_source_object_index
 *     (state+0x8), r8=state->input.ignore_target_object_index (state+0xC) — matching the DB prototype's
 *     real positional order once the dead f1-radius GPR shadow (r5) is accounted for.
 *   - path_find: decompile showed a bogus `(const real_point2d *)debug_use_stored_obstacles` cast and
 *     mislabeled trailing args; disasm (0x8381E288-0x8381E2FC) shows r7=start point (p_start_point, r31),
 *     r8=start_surface_index (r26 — itself a Hex-Rays-mistyped int, see below), r9=goal point (p_point,
 *     r28), r10=goal_surface_index (surface_index, r27), and two single-byte stack slots for
 *     finishing_path (computed per-iteration: true only on the final input step, and only if the caller's
 *     *steps_finish_path was already true) and ignore_optional (constant 0 on the first attempt, constant
 *     1 on the disc_optional_count-gated retry).
 *   - `start_surface_index` (r26 in the decompile) is typed `const real_point2d *` by Hex-Rays but is
 *     assigned from `state->input.start_surface_index` (an int, path_input.h+0x20) in one branch and from
 *     `surface_index` (int, r27) in the other — confirmed via disasm it is a plain int mistyped as a
 *     pointer, not a real pointer. Modeled here as `segment_start_surface_index` (int).
 *   - the `need_recompute` (v28 in the decompile) carry-flag arithmetic
 *     (`x - y + (y^0x80000000)` / `!__CFADD__`) is the standard PPC idiom for a signed `x >= y` compare;
 *     reproduced directly as `step_index >= stored_count`, matching its use (skip recomputing obstacles
 *     for steps already cached by a prior debug-overlay run).
 *
 * The 4624-byte stack scratch buffer (`v56` in the decompile) is dual-purpose: its first ~2568 bytes hold
 * a scratch `obstacles` struct (used only when no debug storage is active), and its last 2048 bytes
 * (offset 2576) are reused as a 128-entry `path_step` scratch array for the per-segment backward chain
 * walk — confirmed via disasm: the chain-walk writes are 16-byte records (int surface_index + 3 floats),
 * exactly `path_step`'s layout, and `sizeof(obstacles)` (2568) plus 8 bytes of alignment padding lands
 * exactly on offset 2576. Modeled here as two pointers into one raw buffer rather than a real union, to
 * keep the reuse explicit. */

#include <stdint.h>
#include <math.h>

#include "headers/path_state.h"
#include "headers/path_step.h"
#include "headers/path_debug_storage.h"
#include "headers/obstacles.h"
#include "headers/obstacle_path.h"
#include "headers/disc_flags.h"
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern void obstacles_new(obstacles *obstacles);
extern void obstacles_get_discs_in_sphere(obstacles *obstacles, const real_point3d *center, float radius, const real_vector3d *movement_direction, int ignore_source_object_index, int ignore_target_object_index);
extern int obstacles_add_disc(obstacles *obstacles, int object_index, int16_t flags, const real_point3d *center, float radius);
extern void obstacles_recompute(obstacles *obstacles, float radius);
extern uint8_t path_find(obstacle_path *path, uint8_t ignore_broken_surfaces, const obstacles *obstacles, float radius, const real_point2d *start, int start_surface_index, const real_point2d *goal, int goal_surface_index, uint8_t finishing_path, uint8_t ignore_optional);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);

uint8_t path_avoid_obstacles(path_state *state, int16_t input_step_count, const path_step *input_steps,
        int16_t *avoided_step_count, path_step *avoided_steps, uint8_t *steps_finish_path)
{
    const collision_bsp *bsp = global_collision_bsp;
    float pathfinding_radius = (state->input.pathfinding_radius <= 0.2f) ? 0.2f : state->input.pathfinding_radius;
    path_debug_storage *debug = state->debug;

    if (debug && !debug->debug_use_stored_obstacles)
        debug->stored_obstacle_step_count = 0;

    if (input_step_count <= 0)
        return 1;

    /* scratch[0..2567] doubles as a plain `obstacles` when no debug storage is active; scratch[2576..4623]
     * is always used, regardless of debug storage, as the per-segment backward chain-walk buffer. */
    unsigned char scratch[4624];
    obstacles *default_obstacles = (obstacles *)scratch;
    path_step *chain_steps = (path_step *)&scratch[2576];  /* sub-buffer of the local scratch, not a DB struct member */
    obstacle_path local_obstacle_path;

    real_point3d segment_goal_point;
    int surface_index = 0; /* holds the current segment's goal surface index; carried into the next
                             * iteration's segment_start_surface_index, matching r27's reuse in disasm. */
    __int16 step_index = 0;

    for (;;)
    {
        obstacles *current_obstacles = default_obstacles;
        obstacle_path *current_obstacle_path = &local_obstacle_path;

        const real_point3d *segment_start_point;
        int segment_start_surface_index;
        if (step_index <= 0)
        {
            segment_start_point = &state->input.start_point;
            segment_start_surface_index = state->input.start_surface_index;
        }
        else
        {
            segment_start_point = &segment_goal_point;
            segment_start_surface_index = surface_index;
        }

        const path_step *current_input_step = &input_steps[step_index];
        surface_index = current_input_step->surface_index;

        real_vector3d direction;
        direction.n[0] = current_input_step->point.n[0] - segment_start_point->n[0];
        direction.n[1] = current_input_step->point.n[1] - segment_start_point->n[1];
        direction.n[2] = current_input_step->point.n[2] - segment_start_point->n[2];
        float direction_length = sqrtf(direction.n[0] * direction.n[0] + direction.n[1] * direction.n[1]
                + direction.n[2] * direction.n[2]);
        if (fabsf(direction_length) >= 0.0001f)
        {
            float inv_length = 1.0f / direction_length;
            direction.n[0] *= inv_length;
            direction.n[1] *= inv_length;
            direction.n[2] *= inv_length;
        }

        unsigned __int8 need_recompute = 1;
        if (debug)
        {
            current_obstacles = &debug->path_obstacles[step_index];
            current_obstacle_path = &debug->path_obstacle_paths[step_index];
            if (debug->debug_use_stored_obstacles)
            {
                int stored_count = debug->stored_obstacle_step_count;
                need_recompute = (step_index >= stored_count);
            }
        }

        if (need_recompute)
        {
            obstacles_new(current_obstacles);
            obstacles_get_discs_in_sphere(current_obstacles, segment_start_point, 4.0f, &direction,
                    state->input.ignore_source_object_index, state->input.ignore_target_object_index);
            if (state->input.attractor_valid)
                obstacles_add_disc(current_obstacles, state->input.attractor_object_index,
                        1u << _disc_optional_bit,
                        &state->input.attractor_point, state->input.attractor_radius);
            obstacles_recompute(current_obstacles, pathfinding_radius);
            if (debug && !debug->debug_use_stored_obstacles)
                debug->stored_obstacle_step_count++;
        }

        unsigned __int8 segment_finishing = (step_index == input_step_count - 1) && *steps_finish_path;

        /* path_find is a 2D (horizontal-plane) solver: the real_point3d args are reinterpreted as
         * real_point2d so only their x,y are read — the authentic Blam idiom, kept load-bearing. */
        unsigned __int8 path_found = path_find(current_obstacle_path, state->input.ignore_broken_surfaces,
                current_obstacles, pathfinding_radius, (const real_point2d *)segment_start_point,
                segment_start_surface_index, (const real_point2d *)&current_input_step->point, surface_index,
                segment_finishing, 0);
        if (!path_found && current_obstacles->disc_optional_count > 0)
            path_found = path_find(current_obstacle_path, state->input.ignore_broken_surfaces, current_obstacles,
                    pathfinding_radius, (const real_point2d *)segment_start_point, segment_start_surface_index,
                    (const real_point2d *)&current_input_step->point, surface_index, segment_finishing, 1);
        if (!path_found)
            return 0;

        if (current_obstacle_path->goal_found_exactly)
        {
            segment_goal_point = current_input_step->point;
        }
        else
        {
            step *goal_step = &current_obstacle_path->steps[current_obstacle_path->goal_step_index];
            collision_surface_project_point2d(bsp, goal_step->surface_index, 2, 1,
                    &goal_step->point, &segment_goal_point);
        }

        unsigned __int8 overflow = 0;
        int chain_count = 0;
        __int16 chain_index = current_obstacle_path->goal_step_index;
        if (chain_index != 0)
        {
            for (;;)
            {
                const step *chain_step = &current_obstacle_path->steps[chain_index];
                path_step *record = &chain_steps[chain_count];
                record->surface_index = chain_step->surface_index;
                collision_surface_project_point2d(bsp, chain_step->surface_index, 2, 1,
                        &chain_step->point, &record->point);
                chain_index = chain_step->previous_step_index;
                chain_count++;
                if (chain_count >= 128)
                {
                    overflow = 1;
                    break;
                }
                if (chain_index == 0)
                    break;
            }
        }

        __int16 output_count = *avoided_step_count;
        int copy_index = chain_count - 1;
        if (copy_index >= 0)
        {
            while (output_count < 4)
            {
                avoided_steps[output_count] = chain_steps[copy_index];
                copy_index--;
                output_count++;
                if (copy_index < 0)
                    goto steps_copied;
            }
            overflow = 1;
        }
    steps_copied:
        *avoided_step_count = output_count;

        if (overflow)
        {
            *steps_finish_path = 0;
            return 1;
        }

        step_index++;
        if (step_index >= input_step_count)
            return 1;
    }
}
