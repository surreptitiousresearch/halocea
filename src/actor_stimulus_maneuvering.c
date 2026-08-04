/* actor_stimulus_maneuvering @0x837D52A0 — issue a maneuvering stimulus to an actor: broadcast an
 * advance (22) or retreat (23) communication event from its unit, and if `flee` is set, raise the actor's
 * flee urgency to at least 6 (clearing the related target reference). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_panic_type.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void actor_stimulus_maneuvering(uint16_t actor_index, uint8_t advancing, uint8_t flee)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.unit_index != -1 )
        ai_communication_event(advancing ? _ai_communication_advance : _ai_communication_retreat, actor->meta.unit_index, -1, -1, -1, -1, nullptr);

    if ( flee )
    {
        if ( actor->stimuli.panic_type < _actor_panic_platoon_retreating )
        {
            actor->stimuli.panic_type = _actor_panic_platoon_retreating;
            actor->stimuli.panic_prop_index = -1;
        }
    }
}
