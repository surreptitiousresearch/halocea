/* actor_combat_disable_bursts @0x837B7920 — extends (never shortens) the actor's combat burst-disable
 * timer (word offset 763, inside actor_datum's action_data union, no DB struct name established for it). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_combat_disable_bursts(uint16_t actor_index, int disable_timer)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    __int16 *timer = &actor->control.burst_disable_timer;  /* word offset 763 = byte 1526, opaque control region */
    if ( *timer <= disable_timer )
        *timer = (__int16)disable_timer;
}
