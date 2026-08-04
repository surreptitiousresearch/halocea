/* path_3d_build_path @0x837D23C0 — builds a trivial one-step `path_result` for a 3D (physics-driven)
 * pathfinder: clears `path`, asks `path_3d_available` whether the straight line from `start_point` to
 * `end_point` is clear, and if so records the result as a single-step path.
 *
 * DEVIATION (resolves a deferral from an earlier session): the decompiler fabricates a bogus 6th
 * parameter `unsigned __int8 *a6` and an unrelated `path_result *path` read from a DIFFERENT register,
 * making it look like two separate buffers are involved. disasm_range(0x837D23C0, 0x837D2490) shows
 * there is only ONE output buffer (r7, the established `path` parameter) — every single store in the
 * function targets it; the "a6"/"path" split was a fabricated phantom parameter, the same class of
 * artifact seen repeatedly this session. Verified field-for-field against `path_result`'s real
 * (types_members-confirmed) layout: `path->endpoint.point` and `path->steps[0].point` both end up
 * holding `end_point`'s own words verbatim — one copied directly from `end_point` itself, the other via
 * `path_3d_available`'s `hit_result_out` parameter (which itself is just a pass-through copy of the
 * same buffer, see `path_3d_available.c`) — genuinely the same value reached through two different
 * plumbing paths, not two different results. `path->endpoint.surface_index`/`target_radius` are fixed
 * sentinels (-1 / 0.0), independent of the collision test's actual outcome. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/path_result.h"

extern uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point, float avoidance_distance, const collision_bsp_test_vector_result *destination_reference, uint8_t *path_available_out, float *hit_result_out);

uint8_t path_3d_build_path(structure_bsp *structure_bsp, const real_point3d *start_point,
        float avoidance_distance, const real_point3d *end_point, path_result *path)
{
    path->valid = 0;
    path->_pad1[0] = 0; path->_pad1[1] = 0; path->_pad1[2] = 0;
    path->endpoint.point.n[0] = 0.0f; path->endpoint.point.n[1] = 0.0f; path->endpoint.point.n[2] = 0.0f;
    path->endpoint.surface_index = 0; path->endpoint.target_radius = 0.0f;
    path->steps_finish_path = 0; path->step_count = 0; path->step_index = 0; path->_pad2 = 0;
    for (int i = 0; i < 4; ++i)
        path->steps[i] = (path_step){ 0, { 0.0f, 0.0f, 0.0f } };

    uint8_t path_available_flag = 0;
    real_point3d hit_result;
    if (path_3d_available(structure_bsp, start_point, avoidance_distance,
            (const collision_bsp_test_vector_result *)end_point, &path_available_flag,
            (float *)&hit_result))
    {
        path->steps_finish_path = path_available_flag;
        path->step_count = 1;
        path->step_index = 0;

        path->endpoint.point = *end_point;
        path->endpoint.surface_index = -1;
        path->endpoint.target_radius = 0.0f;

        path->steps[0].surface_index = -1;
        path->steps[0].point = hit_result;

        path->valid = 1;
    }

    return path->valid;
}
