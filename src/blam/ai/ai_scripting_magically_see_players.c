/* ai_scripting_magically_see_players @0x83772998 — make every actor addressed by an AI index instantly
 * aware of every player's unit. */

#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void ai_scripting_magically_see_unit(int ai_index, int unit_index);

void ai_scripting_magically_see_players(int ai_index)
{
    if ( ai_index == -1 )
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator);
          player;
          player = data_iterator_next(&iterator) )
    {
        ai_scripting_magically_see_unit(ai_index, player->unit_index);
    }
}
