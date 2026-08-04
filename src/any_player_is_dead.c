/* any_player_is_dead @0x836A85E8 — true if at least one active player currently has no unit (dead).
 * The player's controlled unit index lives at player record dword +13 (+52 bytes); -1 means dead. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


uint8_t any_player_is_dead(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return 0;
    while ( player->unit_index != -1 )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return 0;
    }
    return 1;
}
