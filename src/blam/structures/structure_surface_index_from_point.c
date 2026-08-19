/* structure_surface_index_from_point @0x8381CAC0 — re-resolves a point's surface index by casting a
 * zero-length "line" from `known_point`/`known_surface_index` to `*point` against the structure BSP; returns
 * -1 immediately when there's no known surface to cast from, the new surface index on a hit, or falls back to
 * `known_surface_index` when the cast finds nothing (surface_index == -1). `*point` is always overwritten with
 * the cast's resulting (possibly surface-clamped) point. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/real_point2d.h"
#include "headers/path_collision_result.h"

extern uint8_t structure_test_line2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *p0, int p0_surface_index, const real_point2d *p1, int p1_surface_index, path_collision_result *result);

int structure_surface_index_from_point(const structure_bsp *structure, uint8_t ignore_broken_surfaces,
        const real_point2d *known_point, int known_surface_index, real_point2d *point)
{
    if (known_surface_index == -1)
        return -1;

    path_collision_result result;
    structure_test_line2d(structure, ignore_broken_surfaces, known_point, known_surface_index, point, -1, &result);

    point->n[0] = result.point.n[0];
    point->n[1] = result.point.n[1];

    if (result.surface_index == -1)
        return known_surface_index;
    return result.surface_index;
}
