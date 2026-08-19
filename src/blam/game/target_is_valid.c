/* target_is_valid @0x83815160 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


/* return type: DB funcs.prototype = unsigned __int8 (BOOL undefined in C TU) */
uint8_t target_is_valid(int my_index, int old_target_index, int new_target_index)
{
    player_datum *new_target = DATA_ARRAY_ELEMENT(player_data, player_datum, new_target_index);
    player_datum *me = DATA_ARRAY_ELEMENT(player_data, player_datum, my_index);

    if (new_target_index != my_index
        && new_target_index != old_target_index
        && new_target->team_index != me->team_index)
    {
        return new_target->unit_index != -1;
    }
    return 0;
}
