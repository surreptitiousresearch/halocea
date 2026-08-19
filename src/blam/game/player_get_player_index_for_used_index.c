/* player_get_player_index_for_used_index @0x837462F8 — translate a zero-based "used" (dense) player ordinal into
 * the corresponding player_data datum index by walking the valid players in order. Returns -1 if the ordinal is
 * past the last active player. */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int player_get_player_index_for_used_index(int index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    if ( !data_iterator_next(&iterator) )
        return -1;
    while ( index )
    {
        --index;
        if ( !data_iterator_next(&iterator) )
            return -1;
    }
    return iterator.index;
}
