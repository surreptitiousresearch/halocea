/* actor_look_secondary @0x837FA4B8 — request a low-priority "glance" of a given type toward a direction
 * (a prop or a point). Rejects the request if a higher-priority secondary look is already active, if the
 * actor is too unaware for this look type, or — for prop glances — if the prop is dead/invalid or was glanced
 * at too recently. On acceptance, computes a randomized look duration from the character's secondary-look
 * scale range and stores the look type/priority/duration and the target direction into the actor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_datum.h"
#include "headers/direction_specification.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern int game_time_get(void);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern const float global_secondary_look_times[];
extern const int16_t global_secondary_look_priorities[];

uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority,
                                     direction_specification *direction)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    /* character definition tag data for this actor */
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( (actor->state.mode <= _actor_mode_asleep && type < _secondary_look_scripted) || actor->control.secondary_look_type > type )
        return 0;

    uint8_t accepted = 1;
    if ( type >= _secondary_look_scripted || actor->state.action != actor_action_obey || actor->state.action_data.___u0.obey.allow_looking )
    {
        uint8_t in_combat = actor->orders.look.primary_priority >= _primary_priority_locked_aiming;
        if ( actor->orders.look.primary_priority < _primary_priority_locked_aiming || type >= _secondary_look_newly_acknowledged_prop )
        {
            if ( direction->type == _direction_specification_prop )
            {
                prop_datum *prop = datum_try_and_get(prop_data, direction->___u1.prop_index);
                if ( prop )
                {
                    if ( type < _secondary_look_communicating_prop
                      && ((!prop->enemy && !prop->player)
                          || (prop->player && actor->state.mode >= _actor_mode_combat)) )
                    {
                        int now = game_time_get();
                        if ( in_combat )
                        {
                            accepted = 0;
                        }
                        else if ( !prop->in_combat || type < _secondary_look_newly_acknowledged_prop )
                        {
                            int last_look = prop->last_idle_look_time;
                            if ( last_look != -1 )
                                /* deviation: __CFADD__ carry idiom is the signed test now >= last_look+600.
                                 * (Corrected 2026-07 — numerically verified !__CFADD__(a-b,b^0x80000000) ==
                                 * (a>=b); the sense was previously inverted here.) */
                                accepted = now >= last_look + 600;
                        }
                        if ( accepted )
                        {
                            float interest = prop->look_interest;
                            prop->last_idle_look_time = now;
                            if ( prop->last_idle_look_interest > interest )
                                interest = prop->last_idle_look_interest;
                            prop->last_idle_look_interest = interest;
                        }
                    }
                }
                else
                {
                    accepted = 0;
                }
            }
        }
        else
        {
            accepted = 0;
        }
    }
    else
    {
        accepted = 0;
    }

    if ( accepted )
    {
        float duration = global_secondary_look_times[type];
        float max_scale = 2.0f;
        if ( actor->state.mode < _actor_mode_combat || !actor->state.combat_status )
            duration = global_secondary_look_times[type] * 2.0f;

        float min_scale = character->looking.event_look_modifier_lower_bound;
        if ( character->looking.event_look_modifier_lower_bound != 0.0f || character->looking.event_look_modifier_upper_bound != 0.0f )
        {
            if ( min_scale <= 0.5f )
                min_scale = 0.5f;
            else
                min_scale = character->looking.event_look_modifier_lower_bound;
            if ( character->looking.event_look_modifier_upper_bound <= 2.0f )
                max_scale = character->looking.event_look_modifier_upper_bound;
            unsigned int *seed = get_global_random_seed_address();
            duration = real_seed_random_range(seed, min_scale, max_scale) * duration;
        }

        int ticks = (int)(duration * 30.0f);
        if ( ticks > 0x7FFF )
            ticks = 0x7FFF;
        /* _secondary_look_priority_default (1) means "auto": look up the real priority from
         * global_secondary_look_priorities. */
        if ( priority == _secondary_look_priority_default )
            priority = global_secondary_look_priorities[2 * type + (actor->state.combat_status >= _actor_combat_status_certain)];

        actor->control.secondary_look_type     = type;
        actor->control.secondary_look_priority = priority;
        actor->control.secondary_look_timer    = ticks;
        actor->control.secondary_look_direction = *direction;
    }
    return accepted;
}
