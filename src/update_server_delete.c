/* update_server_delete @ 0x836F63B8 — free network update server + client queues */
#include "headers/data_array.h"
#include "headers/data_array.h"
extern void data_dispose(data_array *data);
#include "headers/update_server_globals.h"
#include "headers/update_client_globals.h"
/* DEVIATION: the decompiler modeled both globals with queues@0; the DB-verified layouts place queues@8
 * (server) / @0x58 (client) after the numbered-update fields. Reusing the canonical headers corrects the
 * offsets (field names match). */
void update_server_delete(void)
{
    if ( update_server_globals.queues )
    {
        data_dispose(update_server_globals.queues);
        update_server_globals.queues = 0;
    }
    update_server_globals.initialized = 0;
    update_server_globals.next_update_number_to_build = 0;

    if ( update_client_globals.queues )
    {
        data_dispose(update_client_globals.queues);
        update_client_globals.queues = 0;
    }
    update_client_globals.latest_update_number_received = -1;
    update_client_globals.next_update_number_to_dequeue = 0;
    update_client_globals.initialized = 0;
}
