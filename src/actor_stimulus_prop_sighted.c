/* actor_stimulus_prop_sighted @0x837D40E0 — an actor sighted a prop: if the prop is active (byte @
 * prop->dead == 0) and is an enemy, raise an AI communication event from the actor's unit about the
 * prop's unit, choosing the enemy/ally event type from new_enemy. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/actor_datum.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void actor_stimulus_prop_sighted(uint16_t actor_index, uint16_t prop_index, uint8_t new_enemy)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    if ( !prop->dead )
    {
        int subject_unit_index = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.unit_index;
        if ( subject_unit_index != -1 )
        {
            if ( prop->enemy )
                ai_communication_event(new_enemy ? _ai_communication_sighted_enemy : _ai_communication_found_enemy, subject_unit_index, prop->unit_index, _comm_hostility_enemy, -1, -1, 0);
        }
    }
}
