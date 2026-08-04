/* director_initialize_variables @ 0x836E4930 — reset the four debug "director variables" for a local
 * player to their definitions' initial values, clearing accumulated velocity and delta. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_variable_definition.h"
#include "headers/blam_data_globals.h"

void director_initialize_variables(int16_t local_player_index)
{
    director *dir = &director_globals.local_players[local_player_index];
    int i;

    for ( i = 0; i < 4; i = (__int16)(i + 1) )
    {
        dir->debug_variables[i].value = variables[i].initial_value;
        dir->debug_variables[i].delta = 0.0f;
        dir->debug_variables[i].velocity = 0.0f;
    }
}
