/* projectile_aim @0x83759690 — choose the projectile aiming solver. Uses a ballistic (lobbed) solve when
 * the projectile definition is flagged ballistic (flags @+0x17C bit 0x2) and has a positive gravity scale
 * (+0x1CC); otherwise a straight-line solve. The base velocity is the caller's override (when provided)
 * or the definition's default muzzle velocity (+0x1E4). The chosen solver fills the result outputs; the
 * result_linear byte (when supplied) is set 0 for ballistic, 1 for linear.
 *
 * DEVIATION: the decompiler invents ~30 phantom params for this wrapper. The real 13-arg signature was
 * confirmed cross-referencing the weapon_aim call site (8 registers + 5 stack: result_aim_vector,
 * result_velocity, result_ticks, result_distance, result_linear). The ballistic helper's two extra outputs
 * (vertical/horizontal velocity) are projectile_aim-local scratch. Field offsets and branch logic verified. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/projectile_definition.h"
#include "headers/projectile_definition_flags.h"

extern uint8_t projectile_aim_ballistic(float base_velocity, float gravity_scale, const real_point3d *origin, const real_point3d *target_point, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, float *result_vertical_velocity, float *result_horizontal_velocity);
extern int projectile_aim_linear(float base_velocity, const real_point3d *origin, const real_point3d *target_point, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance);

uint8_t projectile_aim(
    const projectile_definition *projectile_definition,
    const real_point3d *origin,
    const real_point3d *target_point,
    const float *override_velocity_max,
    float *target_velocity_min,
    float *target_ballistic_fraction_min,
    float *forced_velocity,
    uint8_t lob,
    real_vector3d *result_aim_vector,
    float *result_velocity,
    float *result_ticks,
    float *result_distance,
    uint8_t *result_linear)
{
    float base_velocity = override_velocity_max ? *override_velocity_max
                                                : projectile_definition->projectile.initial_velocity;   /* +0x1E4 */

    uint8_t result;
    unsigned int flags = projectile_definition->projectile.flags;                    /* +0x17C */
    float gravity_scale = projectile_definition->projectile.air_gravity_scale;       /* +0x1CC */
    if ( (flags & (1u << _projectile_aim_ballistic_bit)) != 0 && gravity_scale > 0.0f )
    {
        float result_vertical_velocity;
        float result_horizontal_velocity;
        result = projectile_aim_ballistic(base_velocity, gravity_scale, origin, target_point,
            target_velocity_min, target_ballistic_fraction_min, forced_velocity, lob, result_aim_vector,
            result_velocity, result_ticks, result_distance, &result_vertical_velocity, &result_horizontal_velocity);
        if ( result_linear )
            *result_linear = 0;
    }
    else
    {
        result = projectile_aim_linear(base_velocity, origin, target_point, result_aim_vector,
            result_velocity, result_ticks, result_distance);
        if ( result_linear )
            *result_linear = 1;
    }
    return result;
}
