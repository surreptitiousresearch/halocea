#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


uint8_t actor_firing_position_discarded(int actor_index, int16_t firing_position_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( firing_position_index == -1 )
        return 0;

    for ( int i = 0; i < 4; i++ )
    {
        if ( firing_position_index == actor->firing_positions.discarded_firing_positions[i].index )
            return 1;
    }
    return 0;
}
