/* path_find @0x8381DEE8 — seeds an obstacle_path search at `start` and repeatedly calls path_iterate until
 * the search exhausts, then resolves the best reachable step toward `goal` (an exact hit if one was found,
 * otherwise the closest blocked step, if any). Returns whether a usable goal step was found.
 *
 * DEVIATION — the DB's applied prototype only covers the real first 10 parameters; Hex-Rays additionally
 * fabricates 21 phantom trailing parameters (a11..a28, a29, a30, a31) from stack-scanning noise, and worse,
 * the function BODY reads two of those phantoms (`a29`, `a31`) for `path->finishing`/`path->ignore_optional`
 * instead of the real `finishing_path`/`ignore_optional` parameters — even though the decompile's own
 * parameter list already carries the right NAMES for params 1-10, matching them was not enough; ground truth
 * came from disasm_range (0x8381DEE8-0x8381E008): `finishing_path`/`ignore_optional` are read via
 * `lbz r6/r5, arg_57/arg_5F(r1)`, the exact same physical stack offsets the caller (path_avoid_obstacles)
 * stores them at, and stored to `path->finishing`/`path->ignore_optional` (offsets 0x29/0x2A — obstacle_path.h),
 * confirming the phantom-param reads were spurious. `start`/`start_surface_index` (params 5-6) are similarly
 * staged into r28/r27 before the `obstacles_test_circle` call (which clobbers volatile registers) and used
 * unscrambled for the seed `path_add_step` call — the decompile's own naming for those two params is correct,
 * only `a29`/`a31` are wrong.
 *
 * The trailing return-value expression (`goal_step_index + 1 - (goal_step_index + (goal_step_index == -1))`)
 * is algebraically `path->goal_step_index != -1` (verified: reduces to `1 - (goal_step_index == -1)`, i.e.
 * 0 when goal_step_index is -1, 1 otherwise) — reproduced directly rather than the obfuscated form; disasm
 * confirms the raw carry-idiom (`addic`/`subfe`) computes exactly this 0/1 test. */

#include <stdint.h>
#include "headers/obstacle_path.h"
#include "headers/obstacles.h"
#include "headers/real_point2d.h"
#include "headers/structure_bsp.h"

extern int16_t path_add_step(obstacle_path *path, const real_point2d *point, int surface_index, int16_t obstacle_index, uint8_t obstacle_direction_index, float previous_distance, int16_t previous_step_index);
extern uint8_t path_iterate(obstacle_path *path);
extern int16_t obstacles_test_circle(const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *center, float radius);

uint8_t path_find(obstacle_path *path, uint8_t ignore_broken_surfaces, const obstacles *obstacles,
        float radius, const real_point2d *start, int start_surface_index, const real_point2d *goal,
        int goal_surface_index, uint8_t finishing_path, uint8_t ignore_optional)
{
    path->radius = radius;
    path->obstacles = obstacles;
    path->ignore_broken_surfaces = ignore_broken_surfaces;
    path->goal_found_exactly = 0;
    path->structure = global_structure_bsp;
    path->goal = *goal;
    path->goal_surface_index = goal_surface_index;

    __int16 goal_disc_index = obstacles_test_circle(obstacles, -1, goal, radius);
    __int16 goal_obstacle_index = (goal_disc_index == -1) ? -1 : obstacles->discs[goal_disc_index].obstacle_index;

    path->goal_step_index = -1;
    path->goal_obstacle_index = goal_obstacle_index;
    path->best_goal_blocked_step_index = -1;
    path->finishing = finishing_path;
    path->ignore_optional = ignore_optional;
    path->best_goal_blocked_distance = 3.4028235e38f;
    path->step_count = 0;
    path->heap_count = 0;

    path_add_step(path, start, start_surface_index, -1, 0, 0.0f, -1);
    while (path_iterate(path))
        ;

    if (path->goal_step_index == -1)
    {
        if (path->best_goal_blocked_step_index != -1)
            path->goal_step_index = path->best_goal_blocked_step_index;
    }
    else
    {
        path->goal_found_exactly = 1;
    }

    return path->goal_step_index != -1;
}
