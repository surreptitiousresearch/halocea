/* hud_message_to_all @0x83699258 — print `string` to the HUD of every player that owns a local player
 * slot (i.e. every player with a valid local_player_index). */

#include <wchar.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void hud_print_message(int16_t local_player_index, const wchar_t *text);

void hud_message_to_all(uint16_t *string)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for ( player_datum *player = data_iterator_next(&iterator);
          player;
          player = data_iterator_next(&iterator) )
    {
        __int16 local_player_index = player->local_player_index;
        if ( (unsigned __int16)local_player_index != 0xFFFF )
            hud_print_message(local_player_index, (const wchar_t *)string);
    }
}
