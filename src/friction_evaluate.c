/* friction_evaluate @0x837BE368 — decompose a friction force about a surface and recombine it with separate
 * parallel/perpendicular scale factors. The `components` output points at three consecutive vectors
 * (combined result, parallel component, perpendicular component — i.e. a friction_datum). The decomposition
 * basis depends on `type`:
 *   0 — parallel = input, perpendicular = 0 (no scaling applied)
 *   1 — decompose about `primary`
 *   2 — decompose about (secondary x primary)
 *   3 — decompose about `secondary`
 * Types 1-3 then scale parallel by parallel_scale, perpendicular by perpendicular_scale, and sum them.
 *
 * DEVIATION (2026-07-31): six params, not eight. The DB decompile inserted two phantom pointer params
 * ahead of the real ones — a mis-modeling of the f1/f2 float args' reserved r4/r5 GPR shadow slots. Disasm
 * shows six args: type (r3), parallel_scale (f1), perpendicular_scale (f2), the friction_datum output
 * `components` (r6), and the two basis-axis vectors primary (r7) and secondary (r8). The output is a
 * friction_datum (friction/parallel/perpendicular), not the decompiler's real_vector3d*[0..2]; the two
 * axis pointers are real_vector3d* to match the caller (its arg types are already consistent). */

#include <stdint.h>
#include "headers/friction_datum.h"
#include "headers/friction_type.h"
#include "headers/real_vector3d.h"

extern void component_vectors_from_normal3d(const real_vector3d *vector, const real_vector3d *normal, real_vector3d *parallel, real_vector3d *perpendicular);

void friction_evaluate(int16_t type, float parallel_scale, float perpendicular_scale, friction_datum *components, real_vector3d *primary, real_vector3d *secondary)
{
    const real_vector3d *normal;
    real_vector3d cross;

    /* type domain is friction_type (DB $6CBD6362950B8A30A45AF63061A6A1F4: point/forward/left/up);
     * supersedes the earlier BLOCKED adjudication. primary = forward axis, secondary = up axis. */
    switch ( type )
    {
        case _friction_type_point:
            components->parallel.n[0] = components->friction.n[0];
            components->parallel.n[1] = components->friction.n[1];
            components->parallel.n[2] = components->friction.n[2];
            components->perpendicular.n[0] = 0.0f;
            components->perpendicular.n[1] = 0.0f;
            components->perpendicular.n[2] = 0.0f;
            return;
        case _friction_type_forward:
            normal = primary;
            break;
        case _friction_type_left:
            /* cross = secondary x primary */
            cross.n[0] = (secondary->n[1] * primary->n[2]) - (secondary->n[2] * primary->n[1]);
            cross.n[1] = (primary->n[0] * secondary->n[2]) - (secondary->n[0] * primary->n[2]);
            cross.n[2] = (secondary->n[0] * primary->n[1]) - (primary->n[0] * secondary->n[1]);
            normal = &cross;
            break;
        case _friction_type_up:
            normal = secondary;
            break;
        default:
            return;
    }

    component_vectors_from_normal3d(&components->friction, normal, &components->parallel, &components->perpendicular);

    components->parallel.n[0] = parallel_scale * components->parallel.n[0];
    components->parallel.n[1] = parallel_scale * components->parallel.n[1];
    components->parallel.n[2] = parallel_scale * components->parallel.n[2];

    float scaled_perp_x = components->perpendicular.n[0] * perpendicular_scale;
    components->perpendicular.n[0] = components->perpendicular.n[0] * perpendicular_scale;
    components->perpendicular.n[1] = perpendicular_scale * components->perpendicular.n[1];
    components->perpendicular.n[2] = perpendicular_scale * components->perpendicular.n[2];

    components->friction.n[0] = scaled_perp_x + components->parallel.n[0];
    components->friction.n[1] = components->parallel.n[1] + components->perpendicular.n[1];
    components->friction.n[2] = components->parallel.n[2] + components->perpendicular.n[2];
}
