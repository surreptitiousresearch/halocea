/* update_client_new @0x836F5DA8 — allocate and reset the client-side network update queue. Returns 1 on
 * success, or the (still-zero) initialized flag if the queue data array could not be allocated. */

#include <stdint.h>
#include "headers/update_client_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);

uint8_t update_client_new(void)
{
    memset(&update_client_globals, 0, sizeof(update_client_globals));
    update_client_globals.queues = data_new("update client queues", 32, 40);
    if ( !update_client_globals.queues )
        return update_client_globals.initialized;
    memset(update_client_globals.updates, -1, sizeof(update_client_globals.updates));
    update_client_globals.initialized = 1;
    update_client_globals.latest_update_number_received = -1;
    update_client_globals.next_update_number_to_dequeue = 0;
    return 1;
}
