/* actor_action_try_to_evade @0x837F33D8 — decide whether an on-foot actor should side-step (evade) an incoming
 * threat this tick, and if so start the evasion animation. Bails unless the actor is idle-and-available
 * (input.vehicle_index == -1, not mid animation, evasion not inhibited) and has an attended prop. The unit
 * tag's evasion distance (definition[141]) must be positive. The threat direction is the prop's actor_to_prop
 * vector; the actor must be facing roughly toward it (dot > 0.4) — measured in full 3D for swarm/flood-type
 * actors (variant flag 0x200000), otherwise on the normalized 2D horizontal facing. It then asks
 * actor_move_try_evasion_direction for a clear evade direction and, on success, fires the matching evade
 * animation impulse (6, or 7 when the chosen direction came back as 1 = a ledge evade).
 *
 * DEVIATION: actor typed as actor_datum* to name the meta/input fields (input.vehicle_index @+344,
 * input.facing_vector @+372). The evasion out-params were recovered from the call-site disasm: r6=&evade
 * direction (in/out __int16, seeded to 4), r8=&evasion_is_ledge (u8 out), r9=&collision_result — Hex-Rays had
 * mapped the two float shadow slots (evade_distance, ledge height) onto phantom pointer args. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_definition.h"
#include "headers/real_vector2d.h"
#include "headers/path_collision_result.h"
#include "headers/actor_evade.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_move_animation_busy(int actor_index);
extern float normalize2d(real_vector2d *v);
extern uint8_t actor_move_try_evasion_direction(int actor_index, real_vector2d *alignment_vector, float evade_distance, int16_t *evade_direction_reference, float maximum_ledge_height, path_collision_result *evasion_is_ledge, path_collision_result *result);
extern uint8_t unit_test_animation_impulse(int unit_index, int16_t animation_impulse);
extern uint8_t actor_move_animation_impulse(int actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment);

uint8_t actor_action_try_to_evade(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->input.vehicle_index != -1 )
        return 0;
    if ( actor_move_animation_busy(actor_index) )
        return 0;
    if ( actor->control.moving )   /* actor+1284: evasion inhibited */
        return 0;

    int prop_index = actor->target.target_prop_index;   /* actor+624: attended prop index */
    if ( prop_index == -1 )
        return 0;
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    char *unit_object = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum);
    unit_definition *unit_def = TAG_GET(unit_definition, *(int *)unit_object);
    float evade_distance = unit_def->unit.evade_distance;
    if ( evade_distance <= 0.0f )
        return 0;

    /* meta.definition_index is the actor ('actr') tag, not the variant — local was mislabeled. */
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
    if ( (actor_def->flags & (1u << _actor_definition_flying_bit)) != 0 )
    {
        float facing_dot = prop->actor_to_prop.n[0] * actor->input.facing_vector.n[0]
                         + (prop->actor_to_prop.n[2] * actor->input.facing_vector.n[2]
                          + prop->actor_to_prop.n[1] * actor->input.facing_vector.n[1]);
        if ( facing_dot <= 0.40000001f )
            return 0;
    }
    else
    {
        real_vector2d flat_direction = *(real_vector2d *)&prop->actor_to_prop;
        if ( normalize2d(&flat_direction) > 0.0f )
        {
            float facing_dot = actor->input.facing_vector.n[1] * flat_direction.n[1]
                             + flat_direction.n[0] * actor->input.facing_vector.n[0];
            if ( facing_dot <= 0.40000001f )
                return 0;
        }
    }

    real_vector2d alignment_vector = *(real_vector2d *)&prop->actor_to_prop;
    int16_t evade_direction = _actor_evade_random_side;
    normalize2d(&alignment_vector);

    uint8_t evasion_is_ledge;
    path_collision_result collision_result;
    if ( actor_move_try_evasion_direction(actor_index, &alignment_vector, evade_distance,
            &evade_direction, 0.0f, (path_collision_result *)&evasion_is_ledge, &collision_result) )
    {
        int16_t impulse = (evade_direction == _actor_evade_right) + 6;
        if ( unit_test_animation_impulse(actor->meta.unit_index, impulse) )
            return actor_move_animation_impulse(actor_index, impulse, &alignment_vector);
    }
    return 0;
}
