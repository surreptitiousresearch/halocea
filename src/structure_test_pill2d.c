/* structure_test_pill2d @0x8381CB54 — tests a "pill" (thickened 2D segment: start_point/end_point offset by
 * +-radius along the segment's perpendicular) against the structure BSP by casting two straight edge lines
 * (`structure_test_line2d`) on the right and left sides. Both offset points are first clipped into their
 * surface via a same-surface probe cast (so the pill's edges start from valid structure-relative positions even
 * when start/end_surface_index don't exactly contain the offset point). Whichever side registers a nearer
 * collision (`t`) wins; a collision within `radius` of the true `end_point` is treated as clearance rather than
 * a block (the pill's leading edge is allowed to graze the destination). `flags & 1` disables the refinement
 * re-cast that discards a pill-edge collision when the true centerline from the crossing point onward is clear.
 *
 * DEVIATION — the DB's decompile fabricates ~20 trailing garbage int params (a10..a29): `radius` (param 7,
 * float) consumes a dead GPR shadow (r9), which combined with the already-8-GPR-deep signature pushes `result`
 * (param 9) onto the stack (`arg_54`) — Hex-Rays' stack-argument accounting collapses entirely at that point.
 * Rebuilt wholesale from disasm_range (0x8381CB28-0x8381CF50); the already-resolved `structure_test_line2d`
 * call sites carry IDA's own inline `#` argument-role comments (generated against its real applied prototype),
 * which cross-check exactly against register-level tracing throughout.
 *
 * DEVIATION (preserved, not fixed) — when neither offset-line probe registers a real collision (or the winning
 * probe's crossing point is within `radius` of `end_point`), the compiled function does NOT zero/clear
 * `*result` before returning 0: it copies 28 raw bytes starting 4 bytes into the `left_start_point` stack slot,
 * which spills one word early into the adjacent `right_probe` scratch and never reaches `right_probe.t`
 * (disasm-confirmed at 0x8381CF20-0x8381CF3C). This exact byte range is an artifact of this one compilation's
 * stack layout and not reproducible source-level; real callers only consult `result` when the return value is
 * 1, so this is modeled honestly by leaving `*result` untouched (indeterminate) on this path rather than
 * inventing a specific fabricated value. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/real_point2d.h"
#include "headers/path_collision_result.h"

extern float __fsqrts(float x);
extern float __fabs(float x);
extern uint8_t structure_test_line2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *p0, int p0_surface_index, const real_point2d *p1, int p1_surface_index, path_collision_result *result);

uint8_t structure_test_pill2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces,
        const real_point2d *start_point, int start_surface_index, const real_point2d *end_point,
        int end_surface_index, float radius, unsigned int flags, path_collision_result *result)
{
    float dx = end_point->n[0] - start_point->n[0];
    float dy = end_point->n[1] - start_point->n[1];
    float length = __fsqrts(dx * dx + dy * dy);

    if (__fabs(length) < 0.0001f)
        return 0;

    float inv_length = 1.0f / length;
    if (length <= 0.0f)
        return 0;

    /* perpendicular to (dx,dy), rotated so radius offsets to the "right" of start->end */
    float perp_x = -dy * inv_length;
    float perp_y = dx * inv_length;

    real_point2d right_start_point = { { start_point->n[0] + perp_x * radius, start_point->n[1] + perp_y * radius } };
    int right_start_surface_index = start_surface_index;
    if (start_surface_index != -1)
    {
        path_collision_result clip;
        structure_test_line2d(structure, ignore_broken_surfaces, start_point, start_surface_index,
                &right_start_point, -1, &clip);
        right_start_point = *(real_point2d *)&clip.point;
        if (clip.surface_index != -1)
            right_start_surface_index = clip.surface_index;
    }

    real_point2d right_end_point = { { end_point->n[0] + perp_x * radius, end_point->n[1] + perp_y * radius } };
    int right_end_surface_index = -1;
    if (end_surface_index != -1)
    {
        path_collision_result clip;
        structure_test_line2d(structure, ignore_broken_surfaces, end_point, end_surface_index,
                &right_end_point, -1, &clip);
        right_end_point = *(real_point2d *)&clip.point;
        right_end_surface_index = (clip.surface_index == -1) ? end_surface_index : clip.surface_index;
    }

    real_point2d left_start_point = { { start_point->n[0] - perp_x * radius, start_point->n[1] - perp_y * radius } };
    int left_start_surface_index = start_surface_index;
    if (start_surface_index != -1)
    {
        path_collision_result clip;
        structure_test_line2d(structure, ignore_broken_surfaces, start_point, start_surface_index,
                &left_start_point, -1, &clip);
        left_start_point = *(real_point2d *)&clip.point;
        if (clip.surface_index != -1)
            left_start_surface_index = clip.surface_index;
    }

    real_point2d left_end_point = { { end_point->n[0] - perp_x * radius, end_point->n[1] - perp_y * radius } };
    int left_end_surface_index = -1;
    if (end_surface_index != -1)
    {
        path_collision_result clip;
        structure_test_line2d(structure, ignore_broken_surfaces, end_point, end_surface_index,
                &left_end_point, -1, &clip);
        left_end_point = *(real_point2d *)&clip.point;
        left_end_surface_index = (clip.surface_index == -1) ? end_surface_index : clip.surface_index;
    }

    path_collision_result right_probe;
    if (right_start_surface_index == -1)
    {
        right_probe.collision = 0;
    }
    else
    {
        if (structure_test_line2d(structure, ignore_broken_surfaces, &right_start_point, right_start_surface_index,
                    &right_end_point, right_end_surface_index, &right_probe)
                && right_probe.surface_index != -1
                && (flags & 1) == 0)
        {
            path_collision_result refine;
            unsigned __int8 refine_hit = structure_test_line2d(structure, ignore_broken_surfaces,
                    (const real_point2d *)&right_probe.point, right_probe.surface_index,
                    end_point, end_surface_index, &refine);
            if (!refine_hit)
                right_probe.collision = 0;
        }
    }

    path_collision_result left_probe;
    if (left_start_surface_index == -1)
    {
        left_probe.collision = 0;
    }
    else
    {
        if (structure_test_line2d(structure, ignore_broken_surfaces, &left_start_point, left_start_surface_index,
                    &left_end_point, left_end_surface_index, &left_probe)
                && left_probe.surface_index != -1
                && (flags & 1) == 0)
        {
            path_collision_result refine;
            unsigned __int8 refine_hit = structure_test_line2d(structure, ignore_broken_surfaces,
                    (const real_point2d *)&left_probe.point, left_probe.surface_index,
                    end_point, end_surface_index, &refine);
            if (!refine_hit)
                left_probe.collision = 0;
        }
    }

    path_collision_result *chosen;
    if (right_probe.collision)
    {
        if (!left_probe.collision || right_probe.t < left_probe.t)
            chosen = &right_probe;
        else
            chosen = &left_probe;
    }
    else if (left_probe.collision)
    {
        chosen = &left_probe;
    }
    else
    {
        /* neither side collided: see the file-header DEVIATION note — result is left indeterminate */
        return 0;
    }

    float to_end_x = end_point->n[0] - chosen->point.n[0];
    float to_end_y = end_point->n[1] - chosen->point.n[1];
    if (to_end_y * to_end_y + to_end_x * to_end_x < radius * radius)
    {
        /* crossing point is within radius of the true end point: not a real block */
        return 0;
    }

    *result = *chosen;
    return 1;
}
