/* actor_iterator_new @0x83709B84 — initialise a cursor that walks every actor across all encounters (and the
 * encounterless list). active_only restricts iteration to active encounters/actors. No-op if AI is not
 * initialised for the current map. */

#include "headers/data_array.h"
#include "headers/ai_globals.h"
#include "headers/actor_iterator.h"
#include "headers/blam_data_globals.h"

#include <stdint.h>
#include "headers/data_iterator.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);

void actor_iterator_new(actor_iterator *iterator, uint8_t active_only)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    data_iterator_new(&iterator->encounter_iterator, encounter_data);
    iterator->active_only = active_only;
    iterator->next_index = -1;
    iterator->iterated_encounterless_list = 0;
    iterator->index = -1;
}
