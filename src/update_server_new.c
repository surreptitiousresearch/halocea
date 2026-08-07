/* update_server_new @0x836F6310 — allocate and reset the server-side network update queue, then bring up the
 * client queue. Returns 1 on success, or the (still-zero) initialized flag on allocation/client failure. */

#include <stdint.h>
#include <string.h>
#include "headers/update_server_globals.h"

extern data_array *data_new(const char *name, int16_t maximum_count, int16_t size);
extern uint8_t update_client_new(void);

uint8_t update_server_new(void)
{
    memset(&update_server_globals, 0, sizeof(update_server_globals));
    update_server_globals.queues = data_new("update server queues", 32, 100);
    if ( !update_server_globals.queues )
        return update_server_globals.initialized;
    memset(update_server_globals.updates, 0, sizeof(update_server_globals.updates));
    if ( !update_client_new() )
        return update_server_globals.initialized;
    update_server_globals.initialized = 1;
    return 1;
}
