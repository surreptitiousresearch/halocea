/* path_test_pill2d @0x8381D7A8 — cast a 2D "pill" (a ray thickened by `radius`) from `point` along `direction`
 * for up to `distance`, against both the structure BSP's edges and the obstacle discs, keeping whichever hit
 * is nearest. `endpoint_is_goal` shortens the initial distance by `radius` (the pill's own endpoint radius,
 * since the caller only needs to know the pill's leading edge reaches the goal). `ignore_structure` skips all
 * structure-edge testing (used for the very first step of a path, which has no established prior surface to
 * test edges against). The structure test itself is 3 rays per side: a straight-ahead ray from `point`, then
 * for each of the two directions perpendicular to `direction`, first a short `radius`-length probe (used only
 * to find which surface lies at the pill's edge) followed by the real straight-ahead ray recast from that
 * edge point using the probed surface as its start. A winning obstacle-disc hit clears any structure edge_index
 * already recorded (edge and disc/obstacle hits are mutually exclusive in the result). A final, unconditional
 * ray cast at the winning distance re-derives `result->surface_index` (which the edge/disc-hit bookkeeping
 * above never touches). Returns 1 if anything was hit (edge, disc, or the original endpoint distance shortened
 * for the goal), else 0 (and resets `result->distance` back to the original `distance` untouched).
 *
 * DEVIATION — the DB's own decompile fabricates 38 parameters (a14-a38) from a stack-argument-heavy 13-param
 * call (7 in registers, `radius`/`distance` floats each consuming a GPR shadow, then `ignore_structure`/
 * `endpoint_is_goal`/`ignore_optional`/`result` spilling to the stack) and consequently gets nearly the entire
 * body wrong. Fully reconstructed from disasm_range (0x8381D7A8-0x8381DA3C) instead: the DB's own 13-param
 * `funcs.prototype` for this function (matching the extern already declared in path_iterate.c) is correct and
 * used verbatim; only the confused decompiler BODY is discarded. Two of the six real structure_test_ray2d
 * calls (the short `radius`-length "probe" rays used to find each side's edge surface) have their own hit
 * boolean and edge_index/distance fields entirely unused by the caller — only `result.surface_index` from
 * each probe is read back (as the starting surface for that side's real ray cast); reproduced faithfully
 * (including probe B duplicating probe A's inputs exactly except for which point it offsets) rather than
 * deduplicated, since both probes are pure/side-effect-free and the redundancy is presumably inherited from
 * the original source structuring the two sides as parallel, independent blocks. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/obstacles.h"
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/path_test_pill2d_result.h"
#include "headers/structure_test_ray2d_result.h"
#include "headers/obstacles_test_pill_result.h"

extern uint8_t structure_test_ray2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *point, int surface_index, const real_vector2d *direction, float distance, structure_test_ray2d_result *result);
extern uint8_t obstacles_test_pill(const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *point, const real_vector2d *direction, float radius, float distance, uint8_t ignore_optional, obstacles_test_pill_result *result);
extern real_vector2d *perpendicular2d(const real_vector2d *a, real_vector2d *result);

uint8_t path_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces,
        const obstacles *obstacles, int16_t ignore_disc_index, const real_point2d *point, int surface_index,
        const real_vector2d *direction, float radius, float distance, uint8_t ignore_structure,
        uint8_t endpoint_is_goal, uint8_t ignore_optional, path_test_pill2d_result *result)
{
    result->distance = distance;
    result->surface_index = -1;
    result->edge_index = -1;
    result->disc_index = -1;
    result->obstacle_index = -1;

    if (endpoint_is_goal)
        result->distance = distance - radius;

    if (!ignore_structure)
    {
        structure_test_ray2d_result straight_hit;
        if (structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, direction,
                result->distance, &straight_hit)
                && result->distance > straight_hit.distance)
        {
            result->distance = straight_hit.distance;
            result->edge_index = straight_hit.edge_index;
        }

        real_vector2d perp;
        perpendicular2d(direction, &perp);

        /* side A (+perp) */
        {
            real_point2d side_point;
            side_point.n[0] = perp.n[0] * radius + point->n[0];
            side_point.n[1] = perp.n[1] * radius + point->n[1];

            structure_test_ray2d_result probe; /* only probe.surface_index is used */
            structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, &perp, radius, &probe);

            structure_test_ray2d_result side_hit;
            if (structure_test_ray2d(structure, ignore_broken_surfaces, &side_point, probe.surface_index, direction,
                    result->distance, &side_hit)
                    && result->distance > side_hit.distance)
            {
                result->distance = side_hit.distance;
                result->edge_index = side_hit.edge_index;
            }
        }

        /* side B (-perp) */
        {
            real_point2d side_point;
            side_point.n[0] = point->n[0] - perp.n[0] * radius;
            side_point.n[1] = point->n[1] - perp.n[1] * radius;

            structure_test_ray2d_result probe; /* only probe.surface_index is used */
            structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, &perp, radius, &probe);

            structure_test_ray2d_result side_hit;
            if (structure_test_ray2d(structure, ignore_broken_surfaces, &side_point, probe.surface_index, direction,
                    result->distance, &side_hit)
                    && result->distance > side_hit.distance)
            {
                result->distance = side_hit.distance;
                result->edge_index = side_hit.edge_index;
            }
        }
    }

    obstacles_test_pill_result pill_hit;
    if (obstacles_test_pill(obstacles, ignore_disc_index, point, direction, radius, result->distance,
            ignore_optional, &pill_hit)
            && result->distance > pill_hit.distance)
    {
        result->distance = pill_hit.distance;
        result->edge_index = -1;
        result->disc_index = pill_hit.disc_index;
        result->obstacle_index = pill_hit.obstacle_index;
    }

    uint8_t hit_anything;
    if (result->edge_index == -1 && (uint16_t)result->disc_index == 0xFFFF)
    {
        result->distance = distance;
        hit_anything = 0;
    }
    else
    {
        hit_anything = 1;
    }

    structure_test_ray2d_result final_hit;
    structure_test_ray2d(structure, ignore_broken_surfaces, point, surface_index, direction, result->distance,
            &final_hit);
    result->surface_index = final_hit.surface_index;

    return hit_anything;
}
