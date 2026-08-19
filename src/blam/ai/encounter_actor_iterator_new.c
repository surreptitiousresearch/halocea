/* encounter_actor_iterator_new @0x83709A10 — initialise a cursor over the actors of an encounter. A -1
 * encounter iterates the global encounterless-actor list; otherwise it seeds from the encounter's first-actor
 * field. No-op if the AI subsystem is not initialised for the current map. */

#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/blam_data_globals.h"


void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    iterator->encounter_index = encounter_index;
    iterator->index = -1;
    if ( encounter_index == -1 )
        iterator->next_index = ai_globals->first_encounterless_actor_index;
    else
        iterator->next_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->first_actor_index;
}
