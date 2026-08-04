/* actor_handle_communication @0x8371FB30 — apply the informational payload of a heard communication to a
 * listening actor, by information_type: 2 = a friend has entered combat (enter-combat stimulus), 3 = a friend
 * shares knowledge of a target (create an orphan prop from the friend's prop, if this actor's prop has a valid
 * actor and the referenced friend prop still exists), 4 = a friend is fleeing (flee stimulus). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_speech_item.h"  /* ai_information_packet */
#include "headers/prop_datum.h"
#include "headers/ai_information_type.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void actor_stimulus_enter_combat_friend_in_combat(uint16_t actor_index, uint16_t prop_index);
extern void actor_stimulus_prop_fleeing(uint16_t actor_index, uint16_t prop_index);
extern int actor_perception_create_orphan_from_friend(int actor_index, int unit_index, int friend_actor_index, int friend_prop_index);

void actor_handle_communication(int actor_index, int prop_index, ai_information_packet *ai_information)
{
    if ( !ai_information )
        return;

    switch ( (unsigned __int16)ai_information->information_type )
    {
        case _ai_information_combat_stimulus:
            actor_stimulus_enter_combat_friend_in_combat(actor_index, prop_index);
            break;
        case _ai_information_target_knowledge:
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
            if ( prop->actor_index != -1 )
            {
                int friend_prop_index = ai_information->information_data.___u0.target_knowledge.prop_index;
                prop_datum *friend_prop = datum_try_and_get(prop_data, friend_prop_index);
                if ( friend_prop )
                    actor_perception_create_orphan_from_friend(actor_index, friend_prop->unit_index,
                        prop->actor_index, friend_prop_index);
            }
            break;
        }
        case _ai_information_flee:
            actor_stimulus_prop_fleeing(actor_index, prop_index);
            break;
    }
}
