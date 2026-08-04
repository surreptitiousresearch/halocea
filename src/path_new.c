/* path_new @0x8381D6C0 — initialize an obstacle_path search: store the search parameters, copy the goal
 * point/surface index, resolve the goal's obstacle (if it lands inside a disc) via obstacles_test_circle,
 * reset the step/heap counts and best-blocked tracking, then seed the path with its first step at `start`.
 *
 * DEVIATION: the decompiler explodes this into 33 parameters with two of them (start_surface_index,
 * goal_surface_index) mistyped as real_point2d pointers and two pointer params (start, goal) folded into
 * those same slots — a phantom-parameter/type-swap artifact. Disasm confirms the DB's real 11-parameter
 * prototype and the correct register mapping (radius is FPR-shadowed: it consumes r7's slot even though
 * it's passed in f1, pushing start/start_surface_index/goal to r8/r9/r10 and goal_surface_index to the
 * first stack slot). Also per disasm, `finishing_path` doubles as path_add_step's initial
 * `previous_step_index` argument (re-read from the same stack slot) — reproduced verbatim. */

#include <stdint.h>
#include "headers/obstacle_path.h"

extern int16_t obstacles_test_circle(const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *center, float radius);
extern int16_t path_add_step(obstacle_path *path, const real_point2d *point, int surface_index, int16_t obstacle_index, uint8_t obstacle_direction_index, float previous_distance, int16_t previous_step_index);

void path_new(obstacle_path *path, const obstacles *obstacles, const structure_bsp *structure, uint8_t ignore_broken_surfaces, float radius, const real_point2d *start, int start_surface_index, const real_point2d *goal, int goal_surface_index, uint8_t finishing_path, uint8_t ignore_optional)
{
    path->obstacles = obstacles;
    path->structure = structure;
    path->radius = radius;
    path->ignore_broken_surfaces = ignore_broken_surfaces;
    path->goal_found_exactly = 0;

    path->goal = *goal;
    path->goal_surface_index = goal_surface_index;

    __int16 disc_index = obstacles_test_circle(obstacles, -1, goal, radius);
    __int16 goal_obstacle_index = disc_index == -1 ? (__int16)-1 : obstacles->discs[disc_index].obstacle_index;

    path->goal_step_index = -1;
    path->best_goal_blocked_step_index = -1;
    path->goal_obstacle_index = goal_obstacle_index;
    path->finishing = finishing_path;
    path->ignore_optional = ignore_optional;
    path->best_goal_blocked_distance = 3.4028235e38f;
    path->step_count = 0;
    path->heap_count = 0;

    path_add_step(path, start, start_surface_index, -1, 0, 0.0f, (__int16)finishing_path);
}
