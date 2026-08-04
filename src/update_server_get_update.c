/* update_server_get_update @0x836F61F0 — look up a numbered update in the server's 32-entry ring.
 * Returns NULL when the number is in the future or older than the ring can hold. */

#include <stddef.h>
#include "headers/update_server_globals.h"
#include "headers/update.h"

update *update_server_get_update(int update_number)
{
    if ( update_number >= update_server_globals.next_update_number_to_build
      || update_number < update_server_globals.next_update_number_to_build - 32 )
    {
        return NULL;
    }
    return &update_server_globals.updates[update_number & 0x1F];
}
