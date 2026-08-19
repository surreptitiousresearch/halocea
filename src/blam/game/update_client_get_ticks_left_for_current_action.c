/* update_client_get_ticks_left_for_current_action @0x836F6120 */
#include "headers/update_client_globals.h"

int update_client_get_ticks_left_for_current_action(void)
{
    return update_client_globals.client_ticks_to_apply_action_to;
}
