/* encounter_update_timers @0x8370BC78 — per-tick decay of an encounter's timers: two countdown timers
 * (+80, +84) reset to 0 while their associated "active" flags (+69, +68) are set, otherwise incremented by
 * 15 (ticks) unless already at the -1 "stopped" sentinel; a third timer (word +74) decays by 15 (floored at
 * 0) whenever both +71 and +72 flags are set.
 *
 * Fields resolved against encounter_datum: enemy_visible/enemy_alive (timer-active flags), post_combat/
 * post_combat_delay (decay-enable flags), post_combat_delay_timer (decaying), enemy_visible_timer/
 * enemy_alive_timer (incrementing). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


void encounter_update_timers(uint16_t encounter_index)
{
    encounter_datum *encounter = (encounter_datum *)encounter_data->data + encounter_index;

    if ( encounter->enemy_visible )
    {
        encounter->enemy_visible_timer = 0;
    }
    else
    {
        int timer0 = encounter->enemy_visible_timer;
        if ( timer0 != -1 )
            encounter->enemy_visible_timer = timer0 + 15;
    }

    if ( encounter->enemy_alive )
    {
        encounter->enemy_alive_timer = 0;
    }
    else
    {
        int timer1 = encounter->enemy_alive_timer;
        if ( timer1 != -1 )
            encounter->enemy_alive_timer = timer1 + 15;
    }

    if ( encounter->post_combat && encounter->post_combat_delay )
    {
        __int16 decay_timer = encounter->post_combat_delay_timer;
        encounter->post_combat_delay_timer = decay_timer <= 15 ? 0 : decay_timer - 15;
    }
}
