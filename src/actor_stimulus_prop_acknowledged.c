/* actor_stimulus_prop_acknowledged @0x837D3DC8 — react to an enemy prop becoming acknowledged: glance at it,
 * compute a surprise level (scaled up if the actor was unaware and the enemy is shooting at close range or in
 * front of the actor), fire the surprise stimulus, raise a "spotted enemy" AI communication event, and — for
 * a player enemy under the medusa cheat — kill the actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/actor_surprise_type.h"
#include "headers/cheats.h"
#include "headers/actor_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
#include "headers/ai_information_data.h"
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(uint16_t actor_index, int16_t type, int16_t priority, direction_specification *direction);
extern void actor_stimulus_surprise(uint16_t actor_index, int16_t surprise_level, int prop_index, const real_vector3d *surprise_vector);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void actor_kill(int actor_index, uint8_t silent, uint8_t delayed);

void actor_stimulus_prop_acknowledged(int actor_index, int prop_index, uint8_t reappearance,
                                      uint8_t expected)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( !prop->dead )
    {
        direction_specification glance;
        direction_specification empty;
        glance = *direction_get_empty(&empty);
        glance.type = _direction_specification_prop;
        glance.___u1.prop_index = prop_index;
        actor_look_secondary(actor_index, _secondary_look_newly_acknowledged_prop, _secondary_look_priority_default, &glance);

        if ( prop->enemy )
        {
            int surprise = 0;
            __int16 awareness = actor->state.combat_status;
            /* dot of the actor's aiming vector (actor+0x174..) with the prop->actor direction */
            float aim_dot = (actor->input.facing_vector.n[0] * prop->actor_to_prop.n[0])
                          + ((prop->actor_to_prop.n[2] * actor->input.facing_vector.n[2])
                             + (prop->actor_to_prop.n[1] * actor->input.facing_vector.n[1]));
            if ( awareness )
            {
                if ( awareness >= _actor_combat_status_clear_los && aim_dot >= 0.5f )
                    expected = 1;
            }
            else
            {
                expected = 0;
                if ( actor->state.mode < _actor_mode_combat )
                {
                    surprise = prop->shooting != 0;   /* 0 or _actor_surprise_unprepared_enemy_shooting(1) */
                    if ( prop->distance < (double)character->panic.surprise_distance )
                    {
                        __int16 level = prop->shooting != 0;
                        if ( (unsigned __int8)(prop->shooting != 0) <= _actor_surprise_unprepared_enemy_close )
                            level = _actor_surprise_unprepared_enemy_close;
                        surprise = level;
                    }
                }
            }

            if ( !expected && prop->shooting && prop->distance < (double)character->panic.surprise_distance )
            {
                __int16 level = surprise;
                if ( aim_dot >= 0.5f )
                {
                    if ( surprise <= _actor_surprise_unexpected_enemy_close_shooting )
                        level = _actor_surprise_unexpected_enemy_close_shooting;
                }
                else if ( surprise <= _actor_surprise_unexpected_enemy_close_flanked_shooting )
                {
                    level = _actor_surprise_unexpected_enemy_close_flanked_shooting;
                }
                surprise = level;
            }

            if ( surprise > 0 )
                actor_stimulus_surprise(actor_index, surprise, prop_index, &prop->actor_to_prop);

            if ( actor->state.combat_status < _actor_combat_status_definite && !expected && prop->visibility < 2 )
            {
                int actor_unit = actor->meta.unit_index;
                if ( actor_unit != -1 )
                    ai_communication_event(_ai_communication_unexpected_enemy, actor_unit, prop->unit_index, _comm_hostility_enemy, -1, -1, 0);
            }
        }
    }

    if ( prop->player && prop->enemy && !prop->dead && actor->meta.type != _actor_type_mounted_weapon )
    {
        if ( cheat.medusa )
            actor_kill(actor_index, 0, 1u);
    }
}
