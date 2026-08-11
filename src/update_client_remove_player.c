/* update_client_remove_player @0x836F5F40 — drop a player's client-side update queue. */

#include "headers/update_client_globals.h"

extern void datum_delete(data_array *data, int index);

void update_client_remove_player(int player_index)
{
    datum_delete(update_client_globals.queues, player_index);
}
