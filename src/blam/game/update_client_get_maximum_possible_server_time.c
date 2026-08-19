/* update_client_get_maximum_possible_server_time @0x836F6780 — the highest update number the client could
 * advance to: starting at next_update_number_to_dequeue, walk forward while each queued update is complete
 * (1..32 actions) and more updates have been received. The compiled body inlines update_client_get_update
 * (including its fake-update path for pure clients); the recovered helper is called instead. */

#include "headers/update_client_globals.h"
#include "headers/blam_data_globals.h"

extern update *update_client_get_update(int update_number);

int update_client_get_maximum_possible_server_time(void)
{
    int update_number = update_client_globals.next_update_number_to_dequeue;

    while ( update_number <= update_client_globals.latest_update_number_received )
    {
        update *queued_update = update_client_get_update(update_number);
        if ( !queued_update )
            break;
        unsigned int number_of_actions = queued_update->data.number_of_actions;
        if ( !number_of_actions || number_of_actions > 0x20 )
            break;
        ++update_number;
    }
    return update_number;
}
