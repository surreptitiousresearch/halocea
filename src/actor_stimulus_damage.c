/* actor_stimulus_damage @0x837D4408 — react to the actor's unit taking damage: mark it as damaged (+748),
 * raise a level-5 surprise stimulus (toward the attacker's prop direction, or away from the damage velocity
 * when there is no prop), record the damage as a recent "look at the threat" hint, and steer the actor's
 * secondary look toward the prop / away from the hit.
 *
 * Deviation: the decompiler invents a 5th parameter `a5` that is actually the same register (r6) as
 * damage_velocity; both `a5` and the velocity-copy alias `v17` are damage_velocity. damage_fraction (f1) is
 * passed but unused by the body. The recent-threat hint fields are actor tail offsets kept raw.
 *
 * DEVIATION: the post-surprise actor re-fetch + combat-transition write block in the decompiler output is
 * a verbatim compiler-inlined copy of actor_stimulus_combat @0x837D3C90 (zero call xrefs in the binary —
 * it is inlined at every caller), constant-folded for guard_point=NULL — emitted as the call the source
 * had. The NULL guard_point makes guard_point_surface_index/guard_distance dead; -1/0.0f are the assumed
 * folded-away spellings. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/actor_surprise_type.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern void actor_stimulus_surprise(uint16_t actor_index, int16_t surprise_level, int prop_index, const real_vector3d *surprise_vector);
extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);

void actor_stimulus_damage(int actor_index, int prop_index, float damage_fraction,
                           real_vector3d *damage_velocity)
{
    (void)damage_fraction;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = 0;
    const real_vector3d *surprise_vector = 0;
    char have_recoil_direction = 0;
    real_point3d recoil_direction;

    if ( prop_index != -1 )
    {
        prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
        surprise_vector = &prop->actor_to_prop;
    }
    else if ( damage_velocity )
    {
        float speed_sq = (damage_velocity->n[2] * damage_velocity->n[2])
                       + ((damage_velocity->n[0] * damage_velocity->n[0])
                          + (damage_velocity->n[1] * damage_velocity->n[1]));
        if ( speed_sq > 0.25 )
        {
            float inverse_length = -1.0f / __fsqrts(speed_sq);
            recoil_direction.n[0] = damage_velocity->n[0] * inverse_length;
            recoil_direction.n[1] = inverse_length * damage_velocity->n[1];
            recoil_direction.n[2] = inverse_length * damage_velocity->n[2];
            surprise_vector = (const real_vector3d *)&recoil_direction;
            have_recoil_direction = 1;
        }
    }

    actor->stimuli.was_damaged = 1;
    if ( (!prop || prop->enemy) && actor->state.mode < _actor_mode_combat )
    {
        actor_stimulus_surprise(actor_index, _actor_surprise_unprepared_damage, prop_index, surprise_vector);
        actor_stimulus_combat(actor_index, _actor_stimulus_combat_damage, 0, -1, 0.0f, 90,
                              damage_velocity, prop_index, 150, 0);
    }

    direction_specification look_target;
    direction_specification empty;
    look_target = *direction_get_empty(&empty);
    char do_look = 1;
    if ( prop_index == -1 )
    {
        if ( have_recoil_direction )
        {
            look_target.type = _direction_specification_vector;
            look_target.___u1.point = recoil_direction;
        }
        else
        {
            do_look = 0;
        }
    }
    else
    {
        look_target.___u1.prop_index = prop_index;
        look_target.type = _direction_specification_prop;
    }

    if ( do_look )
        actor_look_secondary(actor_index, _secondary_look_damage, _secondary_look_priority_default, &look_target);
}
