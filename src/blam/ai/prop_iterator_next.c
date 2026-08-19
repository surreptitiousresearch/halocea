/* prop_iterator_next @0x837D1C88 — advance a prop cursor: return the current prop (or null at the end) and
 * queue its successor in the actor's prop list. */

#include "headers/data_array.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


prop_datum *prop_iterator_next(prop_iterator *iterator)
{
    int next_index = iterator->next_index;
    iterator->index = next_index;
    prop_datum *result = 0;
    if ( next_index != -1 )
    {
        result = DATA_ARRAY_ELEMENT(prop_data, prop_datum, next_index);
        iterator->next_index = result->next_prop_index;
    }
    return result;
}
