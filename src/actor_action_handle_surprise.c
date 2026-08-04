/* actor_action_handle_surprise @0x837F1B48 — react to a surprise stimulus that meets a minimum surprise level.
 * If the actor is not already suppressed (+352) and its recorded surprise level (+750) is high enough, it plays
 * a flinch/turn animation impulse away from the surprise source: it uses the source direction (+764) when one
 * is set, flipping it (impulse 5 instead of 4) when it points behind the actor's facing, otherwise it uses the
 * fallback direction (+372, impulse 4). It then raises an AI "surprised" communication event (naming the
 * triggering prop's unit and hidden/visible state), and from the actor's definition applies fire-wildly (+144)
 * and burst-disable (+140) timers (seconds -> ticks). Finally it flags the surprise as processed
 * (actor_stimulus_was_surprised), retargets to the prop if any, and returns whether it acted. The surprise
 * level (+750) is always cleared on exit.
 *
 * Fields resolved to named actor_datum/prop_datum members (definition tag index +92, suppressed flag +352,
 * surprise level +750, source directions +372/+764, facing vector +1444, unit index +24, triggering prop
 * index +756; prop unit index +24, hidden flag +96). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_vector2d.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/unit_animation_impulse.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern float normalize2d(real_vector2d *v);
extern uint8_t actor_move_animation_impulse(uint16_t actor_index, int16_t animation_impulse, const real_vector2d *animation_alignment);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void actor_combat_fire_wildly(uint16_t actor_index, int16_t fire_ticks);
extern void actor_combat_disable_bursts(uint16_t actor_index, int disable_timer);
extern void actor_stimulus_was_surprised(uint16_t actor_index);
extern uint8_t actor_situation_try_new_target(int actor_index, int prop_index);

int actor_action_handle_surprise(int actor_index, int16_t minimum_surprise_level)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int result = 0;
    actor_variant_definition *definition =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);

    if ( !actor->input.vehicle_passenger && actor->stimuli.surprise_level >= minimum_surprise_level )
    {
        real_vector2d flinch_direction;
        __int16 animation_impulse;

        if ( !actor->stimuli.surprise_has_vector )
        {
            flinch_direction = *(real_vector2d *)&actor->input.facing_vector;
            normalize2d(&flinch_direction);
            animation_impulse = _unit_animation_impulse_surprise_front;
        }
        else
        {
            flinch_direction = *(real_vector2d *)&actor->stimuli.surprise_vector;
            normalize2d(&flinch_direction);
            if ( ((actor->control.desired_facing_vector.n[1] * flinch_direction.n[1])
                       + (actor->control.desired_facing_vector.n[0] * flinch_direction.n[0])) >= 0.0f )
            {
                animation_impulse = _unit_animation_impulse_surprise_front;
            }
            else
            {
                flinch_direction.n[0] = -flinch_direction.n[0];
                flinch_direction.n[1] = -flinch_direction.n[1];
                animation_impulse = _unit_animation_impulse_surprise_back;
            }
        }

        actor_move_animation_impulse(actor_index, animation_impulse, &flinch_direction);

        int cause_unit_index = -1;
        __int16 cause_hostility = _comm_hostility_none;
        if ( actor->stimuli.surprise_prop_index != -1 )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->stimuli.surprise_prop_index);
            cause_unit_index = prop->unit_index;
            cause_hostility = prop->enemy ? _comm_hostility_enemy : _comm_hostility_friend;
        }
        ai_communication_event(_ai_communication_surprise, actor->meta.unit_index, cause_unit_index, cause_hostility, -1, -1, 0);

        if ( definition->ranged_combat.surprise_wildfire_time > 0.0f )
            actor_combat_fire_wildly(actor_index,
                (int)(definition->ranged_combat.surprise_wildfire_time * (float)30.0));
        if ( definition->ranged_combat.surprise_delay_time > 0.0f )
            actor_combat_disable_bursts(actor_index,
                (int)(definition->ranged_combat.surprise_delay_time * (float)30.0));

        actor_stimulus_was_surprised(actor_index);
        int prop_index = actor->stimuli.surprise_prop_index;
        if ( prop_index != -1 )
            actor_situation_try_new_target(actor_index, prop_index);
        result = 1;
    }

    actor->stimuli.surprise_level = 0;
    return result;
}
