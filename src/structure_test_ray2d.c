/* structure_test_ray2d @0x8381C420 — cast a 2D ray from `point` along `direction` for up to `distance`,
 * starting inside `surface_index`, walking through the structure BSP's surface boundary loops wherever the
 * ray exits through a "passable" edge (a pathfinding-surface flag set, and not blocked by a broken/breakable
 * surface unless `ignore_broken_surfaces`) into the neighboring surface, and stopping either when the ray
 * runs out of `distance` inside some surface (returns 0, `edge_index = -1`) or hits an edge it can't pass
 * through (returns 1, `edge_index` = the edge that stopped it).
 *
 * DEVIATION — dead-float-shadow-GPR pattern: `distance` (param 6, float) consumes a dead GPR shadow, so the
 * DB's own decompile fabricates a trailing 8th parameter (`a8`) that receives every write, while its declared
 * 7th parameter (`result`) is printed as unused. disasm_range (0x8381C420-0x8381C47C) shows this function only
 * has 7 real parameter slots (r3,r4,r5,r6,r7,f1-shadowing-r8,r9) with `result` correctly bound to r9 — `a8`
 * and `result` are the SAME physical register; this is the real 7-param signature already used at every call
 * site throughout this project's path_* arc. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/structure_test_ray2d_result.h"
#include "headers/collision_surface_test_line2d_result.h"
#include "headers/PATHFINDING_SURFACE_flags.h"
#include "headers/bit_vector.h"

extern const uint8_t *breakable_surface_flags_get(void);
extern uint8_t collision_surface_test_line2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point, const real_vector2d *vector, collision_surface_test_line2d_result *result);

static uint8_t structure_test_ray2d_surface_passable(const unsigned char *pathfinding_surfaces,
        const collision_surface *surfaces, const unsigned char *breakable_flags, uint8_t ignore_broken_surfaces,
        int surface_index)
{
    if (surface_index == -1 || !pathfinding_surfaces[surface_index])
        return 0;

    uint8_t passable = 1;
    if (!ignore_broken_surfaces && (pathfinding_surfaces[surface_index] & (1u << PATHFINDING_SURFACE_BREAKABLE_BIT)) != 0)
    {
        unsigned char breakable_index = surfaces[surface_index].breakable_surface_index;
        passable = BIT_VECTOR_TEST_FLAG((const unsigned int *)breakable_flags, breakable_index);
    }
    return passable;
}

uint8_t structure_test_ray2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces,
        const real_point2d *point, int surface_index, const real_vector2d *direction, float distance,
        structure_test_ray2d_result *result)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;
    const unsigned char *breakable_flags = breakable_surface_flags_get();
    const unsigned char *pathfinding_surfaces = (const unsigned char *)structure->pathfinding_surfaces.address;
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;

    collision_surface_test_line2d_result test;
    collision_surface_test_line2d(bsp, surface_index, 2, 1u, point, direction, &test);

    for (;;)
    {
        int next_surface_index = -1;

        if (distance < test.enter_t
                && structure_test_ray2d_surface_passable(pathfinding_surfaces, surfaces, breakable_flags,
                        ignore_broken_surfaces, test.enter_surface_index)
                && test.enter_surface_index != -1)
        {
            next_surface_index = test.enter_surface_index;
        }
        else if (distance > test.exit_t
                && structure_test_ray2d_surface_passable(pathfinding_surfaces, surfaces, breakable_flags,
                        ignore_broken_surfaces, test.exit_surface_index)
                && test.exit_surface_index != -1)
        {
            next_surface_index = test.exit_surface_index;
        }
        else
        {
            break;
        }

        surface_index = next_surface_index;
        collision_surface_test_line2d(bsp, surface_index, 2, 1u, point, direction, &test);
    }

    if (distance >= test.enter_t)
    {
        if (distance <= test.exit_t)
        {
            result->distance = distance;
            result->surface_index = surface_index;
            result->edge_index = -1;
            return 0;
        }

        result->distance = test.exit_t;
        result->surface_index = surface_index;
        result->edge_index = test.exit_edge_index;
        return 1;
    }

    result->distance = test.enter_t;
    result->surface_index = surface_index;
    result->edge_index = test.enter_edge_index;
    return 1;
}
