/* actor_stimulus_abandon_stationary_facing @0x837D5390 — if an actor is holding a stationary facing and
 * its character definition allows abandoning it (abandon time @ tag +820 > 0), clear the holding flag and
 * arm the abandon timer (time * 30 ticks, actor word +858). If the actor's perceived enemy prop is very
 * close (prop->distance < 4.0), bump the actor's reaction value (emotions.perceived_danger) up to at
 * least 1.8. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_abandon_stationary_facing(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( actor->emotions.defensive_crouch )
    {
        if ( character->defensive.change_facing_stand_time > 0.0f )
        {
            int prop_index = actor->target.target_prop_index;
            actor->emotions.defensive_crouch = 0;
            actor->emotions.defensive_crouch_timer = (int)(character->defensive.change_facing_stand_time * 30.0f);
            if ( prop_index != -1 && (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index))->distance < 4.0f )
            {
                float reaction = actor->emotions.perceived_danger;
                if ( reaction <= 1.8f )
                    reaction = 1.8f;
                actor->emotions.perceived_danger = reaction;
            }
        }
    }
}
