/* actor_move_animation_busy @0x837C73A8 — true if the actor cannot accept a new movement animation
 * impulse: it has one pending (word @ actor +1048 != 0xFFFF), or its unit (dword @ actor +24) is busy.
 * Actor records are stride 1828. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_is_busy(int object_index);

uint8_t actor_move_animation_busy(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( (unsigned short)actor->orders.move.animation.impulse == 0xFFFF )
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index == -1 || !unit_is_busy(unit_index) )
            return 0;
    }
    return 1;
}
