/* players_in_game @0x83746298 — number of active player data (count of valid entries in the player_data pool). */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int players_in_game(void)
{
    int count = 0;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    while ( data_iterator_next(&iterator) )
        ++count;
    return count;
}
