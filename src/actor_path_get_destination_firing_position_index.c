/* actor_path_get_destination_firing_position_index @0x837C8768 — the actor's cached firing-position index,
 * valid only when actor->control.path.destination_orders.destination_type is 3. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


int actor_path_get_destination_firing_position_index(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->control.path.destination_orders.destination_type == _destination_firing_position )
        return (unsigned __int16)actor->control.path.destination_orders.___u3.firing_position_index;
    return -1;
}
