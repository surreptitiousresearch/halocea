/* update_client_get_update @0x836F6230 — return the client-side update record for a given update number. On a
 * real network connection (server, none, or local) the number must fall inside the 128-entry ring window that
 * starts at next_update_number_to_dequeue, otherwise null. Off that path (a pure client) it hands out a rolling
 * fake update slot so prediction can proceed. */

#include <stdint.h>
#include "headers/update_client_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);

static int s_fake_update_number;

update *update_client_get_update(int update_number)
{
    if (game_connection() == _game_connection_network_server || !game_connection() || game_connection() == _game_connection_film_playback)
    {
        if (update_number < update_client_globals.next_update_number_to_dequeue
            || update_number >= update_client_globals.next_update_number_to_dequeue + 128)
            return nullptr;
        return &update_client_globals.updates[update_number & 0x7F];
    }
    else
    {
        return &update_client_globals.updates[s_fake_update_number++ & 0x7F];
    }
}
