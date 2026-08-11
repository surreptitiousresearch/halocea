/* closest_available_point_on_surface @0x837D2880 — projects `target_point` onto `surface_index` of the
 * structure's collision BSP (find-closest-2D then re-project to 3D), and returns the Euclidean distance
 * from the target to the resulting closest point.
 *
 * DEVIATION: the decompiler declared `target_point` as `real_point2d *` and read a fake third coordinate
 * via `target_point[1].n[0]` (an out-of-bounds element access at a 2-float stride). Disasm confirms the
 * real parameter type is `real_point3d *` (matching the DB prototype) and that access is really just
 * `target_point->n[2]` (z, at byte offset 8) — a coincidental-stride artifact, not a real second point. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
extern float __fsqrts(float);

extern int collision_surface_find_closest_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point, real_point2d *result);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);

float closest_available_point_on_surface(const structure_bsp *structure, int surface_index,
    const real_point3d *target_point, real_point3d *closest_point)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;
    real_point2d closest_point_2d;

    collision_surface_find_closest_point2d(bsp, surface_index, 2, 1u, (const real_point2d *)target_point, &closest_point_2d);
    collision_surface_project_point2d(bsp, surface_index, 2, 1u, &closest_point_2d, closest_point);

    float dx = closest_point->n[0] - target_point->n[0];
    float dy = closest_point->n[1] - target_point->n[1];
    float dz = closest_point->n[2] - target_point->n[2];

    return __fsqrts(dy * dy + dx * dx + dz * dz);
}
