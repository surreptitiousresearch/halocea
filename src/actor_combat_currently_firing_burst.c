/* actor_combat_currently_firing_burst @0x837B7988 — true when the actor is mid burst-fire: its burst timer
 * (short at actor-datum +1548) is positive and its firing state (ushort at +1522) equals 2 (the "firing"
 * sub-state). Offsets are raw within the 1828-byte actor datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_fire_state.h"
#include "headers/actor_fire_target_type.h"
#include "headers/blam_data_globals.h"


uint8_t actor_combat_currently_firing_burst(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->control.current_fire_target_type > actor_fire_target_none )
        return (uint16_t)actor->control.fire_state == actor_fire_state_bursting;
    return 0;
}
