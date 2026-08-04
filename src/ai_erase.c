/* ai_erase @0x836E7980 — erase actors from the simulation. With encounter_index == -1, every actor is
 * erased; otherwise only actors of the given encounter are erased, optionally narrowed to a specific
 * platoon and/or squad. No-op until the AI system is initialized for the current map. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_iterator.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);
extern actor_datum *encounter_actor_iterator_next(encounter_actor_iterator *iterator);
extern void actor_erase(int actor_index, uint8_t immediate);

void ai_erase(int encounter_index, int platoon_index, int squad_index, uint8_t immediate)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    if ( encounter_index == -1 )
    {
        actor_iterator iterator;
        actor_iterator_new(&iterator, 0);
        while ( actor_iterator_next(&iterator) )
            actor_erase(iterator.index, immediate);
    }
    else
    {
        encounter_actor_iterator iterator;
        encounter_actor_iterator_new(&iterator, encounter_index);
        for ( actor_datum *actor = encounter_actor_iterator_next(&iterator);
              actor;
              actor = encounter_actor_iterator_next(&iterator) )
        {
            if ( (platoon_index == -1 || actor->meta.platoon_index == platoon_index)
              && (squad_index == -1 || actor->meta.squad_index == squad_index) )
            {
                actor_erase(iterator.index, immediate);
            }
        }
    }
}
