/* banlist_free @ 0x83766038 */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"


void banlist_free(void)
{
    dynamic_array_delete(&banned_players);
}
