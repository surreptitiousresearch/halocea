/* structure_surfaces_are_equivalent @0x8381C620 — two surfaces are considered "the same" for pathfinding
 * purposes if they are both valid and their Z heights directly above `destination_point` differ by less than
 * 0.05 world units (effectively: same floor, different triangle). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/collision_bsp.h"

extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);
extern double __fabs(double x);

uint8_t structure_surfaces_are_equivalent(const structure_bsp *structure, const real_point2d *destination_point,
        int destination_surface_index, int test_surface_index)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;

    if (destination_surface_index == -1 || test_surface_index == -1)
        return 0;

    real_point3d destination_point3d;
    real_point3d test_point3d;
    collision_surface_project_point2d(bsp, destination_surface_index, 2, 1u, destination_point, &destination_point3d);
    collision_surface_project_point2d(bsp, test_surface_index, 2, 1u, destination_point, &test_point3d);

    return __fabs(destination_point3d.n[2] - test_point3d.n[2]) < 0.050000001f;
}
