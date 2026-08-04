/* projectile_aim_linear @0x837595A8 — straight-line (non-ballistic) projectile aim solve.
 * Produces the unit aim vector from origin toward target, the straight-line distance, and the
 * flight time (distance / muzzle velocity). Each scalar output is optional. Always returns true.
 *
 * DEVIATION: the decompiler invents an unused r3 `origin` param and shifts every argument by one.
 * r3 is only the return register here; the real first pointer arg is in r4. The 7-arg signature
 * (base_velocity, origin, target_point, result_aim_vector, result_velocity, result_ticks,
 * result_distance) matches the extern already declared by the projectile_aim call site. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float x);
extern float __fabs(float x);

/* return attested int: li r3,1 on the only return path, 0 sampled consumers (net_graph_initialize precedent) */
int projectile_aim_linear(
    float base_velocity,
    const real_point3d *origin,
    const real_point3d *target_point,
    real_vector3d *result_aim_vector,
    float *result_velocity,
    float *result_ticks,
    float *result_distance)
{
    float aim[3];
    float distance;
    float ticks = 0.0f;

    aim[0] = target_point->n[0] - origin->n[0];
    aim[1] = target_point->n[1] - origin->n[1];
    aim[2] = target_point->n[2] - origin->n[2];

    distance = __fsqrts((aim[2] * aim[2]) + ((aim[0] * aim[0]) + (aim[1] * aim[1])));

    if (__fabs(distance) < 0.000099999997f)
    {
        /* Degenerate: leave aim[] as the raw delta and report zero distance. */
        distance = 0.0f;
    }
    else
    {
        float inv_distance = 1.0f / distance;
        aim[0] *= inv_distance;
        aim[1] *= inv_distance;
        aim[2] *= inv_distance;
    }

    if (base_velocity > 0.0f)
        ticks = distance / base_velocity;

    result_aim_vector->n[0] = aim[0];
    result_aim_vector->n[1] = aim[1];
    result_aim_vector->n[2] = aim[2];

    if (result_distance)
        *result_distance = distance;
    if (result_velocity)
        *result_velocity = base_velocity;
    if (result_ticks)
        *result_ticks = ticks;

    return 1;
}
