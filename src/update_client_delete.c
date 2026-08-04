/* update_client_delete @0x836F5E50 — tear down the client-side update prediction/replay system: dispose the
 * pending-update queue data-array and reset the received/dequeued update counters and the initialized flag. */

#include "headers/update_client_globals.h"
#include "headers/data_array.h"

extern void data_dispose(data_array *data);

void update_client_delete(void)
{
    if (update_client_globals.queues)
    {
        data_dispose(update_client_globals.queues);
        update_client_globals.queues = 0;
    }

    update_client_globals.latest_update_number_received = -1;
    update_client_globals.next_update_number_to_dequeue = 0;
    update_client_globals.initialized = 0;
}
