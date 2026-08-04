/* game_engine_player_removed @0x837505D4 — announce a player's departure: for every active player, build
 * the localized "player left" chat message (multiplayer message 28) and post it to the HUD chat box, then
 * invoke the game engine's player_removed hook (message = game_engine_message_quit).
 * DEVIATION: the bare game_connection() call has its result discarded (side-effect/ICF-folded), reproduced. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_engine.h"
#include "headers/player_datum.h"
#include "headers/game_engine_message.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t multiplayer_message_internal(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void hud_chat_box_add_message(const wchar_t *message);
extern int16_t game_connection(void);

void game_engine_player_removed(int player_index)
{
    data_iterator iterator;
    unsigned __int16 message[1048];

    data_iterator_new(&iterator, player_data);
    while ( data_iterator_next(&iterator) )
    {
        int index = iterator.index;
        player_datum *player = datum_try_and_get(player_data, iterator.index);
        if ( player )
        {
            if ( (unsigned __int16)player->local_player_index != 0xFFFF
                    && multiplayer_message_internal(index, game_engine_message_quit, player_index, message, 1024) )
            {
                message[1023] = 0;
                hud_chat_box_add_message(message);
            }
            game_connection();
        }
    }

    void (*player_removed)(int) = game_engine->player_removed;
    if ( player_removed )
        player_removed(player_index);
}
