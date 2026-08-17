/* update_client_get_maximum_actions @0x836F6048 */
#include "headers/update_client_globals.h"

int update_client_get_maximum_actions(void)
{
    return update_client_globals.latest_update_number_received
         - update_client_globals.next_update_number_to_dequeue
         + 1;
}
