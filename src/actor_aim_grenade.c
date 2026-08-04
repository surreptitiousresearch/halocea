/* actor_aim_grenade @0x837B94E8 — compute the throw-direction vector for an actor's grenade and return the
 * target unit index (or -1).
 *
 * If the actor has a grenade "prop" target (+1716) of the right kind (state in [2,3]), the target's body
 * position (+188/+192/+196) plus a small vertical lift (+0.2 on z) is used as the aim point. That point is
 * validated against collateral-damage limits (actor_combat_check_collateral_damage); if it passes, it is
 * latched into actor+1704/1708/1712. The aim is then refreshed (actor_combat_reaim_grenade).
 *
 * The output direction is then chosen:
 *   - if the actor is NOT in a vehicle (input.vehicle_index == -1): the refreshed planar aim vector
 *     (+1724/+1728/+1732) is used; when the aim is more than 30 degrees from the actor's facing
 *     (+372/+376/+380) (facing.aim < cos30 = 0.8660254), the facing vector is rotated toward the aim
 *     and rescaled to the planar aim length;
 *   - otherwise (in a vehicle): the latched aim point above is used directly.
 * The chosen direction is scaled by the actor's throw speed (+1736) into *vector.
 *
 * Deviation: the decompiler packs the working direction into an __int64 (v26) + float (v27); the byte order is
 * straightforward big-endian (no x/y swap) — direction.x = +188/aim_x, direction.y = +192/aim_y,
 * direction.z = +196+0.2 / aim_z. The collateral-damage call's true arguments (per disasm at 0x837B95C8) are
 * (actor_index, variant_def[98], variant_def[103], test_point [r6], NULL [r7]); the decompiler's
 * tag-instance/tag-index arguments are the float GPR-skip phantoms and are not real. The two
 * radii are the variant definition's grenade_combat.enemy_radius / .collateral_damage_radius. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);
extern float __fabs(float);

extern uint8_t actor_combat_check_collateral_damage(int actor_index, float enemy_radius, float collateral_damage_radius, const float *test_point, int16_t *threat_count_out);
extern int actor_combat_reaim_grenade(int actor_index, const real_point3d *grenade_origin);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

int actor_aim_grenade(int actor_index, const real_point3d *origin, real_vector3d *vector)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int target_unit_index = -1;

    float direction_x = 0.0f, direction_y = 0.0f, direction_z = 0.0f;

    int prop_index = actor->control.grenade_current_prop_index;
    if ( prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        __int16 prop_state = prop->state;
        if ( prop_state >= _prop_state_becoming_unacknowledged && prop_state <= _prop_state_acknowledged )
            target_unit_index = prop->unit_index;
        if ( (unsigned __int16)prop_state >= _prop_state_becoming_unacknowledged )
        {
            float aim_point_x = prop->body_position.n[0];
            float aim_point_y = prop->body_position.n[1];
            float aim_point_z = prop->body_position.n[2] + 0.2f;
            direction_x = aim_point_x;
            direction_y = aim_point_y;
            direction_z = aim_point_z;

            /* the actor's variant definition tag (NOT a grenade tag); the two radii are
             * grenade_combat.enemy_radius (was raw variant_def[98] = abs +392) and
             * grenade_combat.collateral_damage_radius (was variant_def[103] = abs +412). */
            int variant_tag_index = actor->meta.variant_definition_index;
            actor_variant_definition *variant_def =
                TAG_GET(actor_variant_definition, variant_tag_index);
            real_point3d test_point = { aim_point_x, aim_point_y, aim_point_z };
            if ( actor_combat_check_collateral_damage(actor_index,
                                                      variant_def->grenade_combat.enemy_radius,
                                                      variant_def->grenade_combat.collateral_damage_radius,
                                                      (const float *)&test_point, 0) )
            {
                actor->control.grenade_current_target.x = aim_point_x;
                actor->control.grenade_current_target.y = aim_point_y;
                actor->control.grenade_current_target.z = aim_point_z;
            }
        }
    }

    actor_combat_reaim_grenade(actor_index, origin);

    if ( actor->input.vehicle_index == -1 )
    {
        float aim_x = actor->control.grenade_current_aim_vector.n[0];
        float aim_y = actor->control.grenade_current_aim_vector.n[1];
        float aim_z = actor->control.grenade_current_aim_vector.n[2];
        direction_x = aim_x;
        direction_y = aim_y;
        direction_z = aim_z;

        float planar_length = __fsqrts(aim_y * aim_y + aim_x * aim_x);
        if ( __fabs(planar_length) >= 0.000099999997f && planar_length > 0.0f )
        {
            float norm_x = aim_x * (1.0f / planar_length);
            float norm_y = aim_y * (1.0f / planar_length);
            float facing_x = actor->input.facing_vector.n[0];
            float facing_y = actor->input.facing_vector.n[1];
            float facing_z = actor->input.facing_vector.n[2];
            if ( (facing_y * norm_y) + (facing_x * norm_x) < 0.86602539f )
            {
                real_vector3d rotated;
                rotated.n[0] = facing_x;
                rotated.n[1] = facing_y;
                rotated.n[2] = facing_z;
                float sign = ((facing_x * norm_y) - (facing_y * norm_x)) <= 0.0f ? -1.0f : 1.0f;
                rotate_vector_about_axis(&rotated, global_up3d, sign * 0.5f, 0.86602539f);
                direction_x = rotated.n[0] * planar_length;
                direction_y = rotated.n[1] * planar_length;
                /* direction_z stays aim_z */
            }
        }
    }

    float throw_speed = actor->control.grenade_current_aim_speed;
    vector->n[0] = throw_speed * direction_x;
    vector->n[1] = direction_y * throw_speed;
    vector->n[2] = throw_speed * direction_z;
    return target_unit_index;
}
