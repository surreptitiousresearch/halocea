/* player_ui_get_single_player_local_player_controller @0x83698868 */
#include <stdint.h>
#include "headers/player_ui_globals.h"

/* param extsh'd (int16_t); return consumed via extsh by all 10 callers (int16_t).
 * Field is __int16, so the plain array read models the lhzx load with no cast. */
int16_t player_ui_get_single_player_local_player_controller(int16_t local_player_index)
{
    return player_ui_globals.single_player_controller_index[local_player_index];
}
