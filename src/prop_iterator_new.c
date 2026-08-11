/* prop_iterator_new @0x837D1C60 — start a cursor over an actor's prop list at the actor's first prop. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_iterator.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void prop_iterator_new(prop_iterator *iterator, int actor_index)
{
    /* was raw *((_DWORD*)actor_data->data + 457*actor_index + 20): actor_datum.meta.first_prop_index @0x50 */
    iterator->next_index = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.first_prop_index;
}
