/* actor_action_allow_cover_seeking @0x837F2580 — whether an actor may currently seek cover: always denied
 * while it's a vehicle passenger or the actor's berserk flag (emotions.berserk) is set. Unless the caller says the
 * threat is unopposable or the actor's external orders mark it playfighting, further denies it while
 * combat_status is "certain" (>=7) and, when target.target_last_visible_time (+620) is set and the unit
 * definition's intimidation duration (+728, `float` seconds) hasn't yet elapsed
 * since it was set, or while the unit definition's danger threshold (+804) is positive and the actor's
 * recent shield damage hasn't reached it.
 *
 * DEVIATION: the decompiler renders the elapsed-time check via a `__CFADD__` carry-flag intrinsic on a
 * subtraction; algebraically this is just `game_time_get() >= (duration_ticks + timestamp)` — reproduced
 * as the plain comparison. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);

uint8_t actor_action_allow_cover_seeking(int actor_index, uint8_t unopposable)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned __int8 allow = 1;

    actor_definition *unit_definition = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( !unopposable && !actor->external_orders.playfighting )
    {
        float intimidation_duration = unit_definition->defensive.hide_target_not_visible_time;
        if ( intimidation_duration > 0.0f )
        {
            if ( actor->state.combat_status < _actor_combat_status_visible )
            {
                /* recovered: raw dword offset 620 -> actor->target.target_last_visible_time (prior "intimidation-start" was a guess) */
                int intimidation_start_time = actor->target.target_last_visible_time;
                if ( intimidation_start_time != -1 )
                {
                    int duration_ticks = (int)(intimidation_duration * 30.0f);
                    allow = game_time_get() >= duration_ticks + intimidation_start_time;
                }
            }
            else
            {
                allow = 0;
            }
        }

        float danger_threshold = unit_definition->defensive.cover_damage_threshold;
        if ( danger_threshold > 0.0f && actor->input.recent_shield_damage < danger_threshold )
            allow = 0;
    }

    /* recovered: raw *((char *)actor + 888) -> emotions.berserk */
    return actor->input.vehicle_passenger == 0 && actor->emotions.berserk == 0 && allow;
}
