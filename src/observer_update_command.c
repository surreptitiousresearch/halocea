/* observer_update_command @ 0x8370EC58 — reconcile the just-produced (pending) camera command against
 * the previous (last) command's per-parameter interpolation timers, so each parameter eases toward its
 * new target rather than snapping. Runs only when the pending command is valid (flags bit0).
 *
 * For each of the 5 interpolated parameters: if the parameter is being interpolated (flag bit0) and is
 * not pinned (flag bit1), and the pending timer is shorter than the last timer, the pending timer is
 * pulled up toward the last timer (capped at 2.0 s). If the parameter is not currently interpolating,
 * the pending timer is set from the command's overall timer when that is at least the last timer (or the
 * command forces it, flags bit3), otherwise from the last timer (capped at 2.0 s). After processing all
 * parameters the pending command becomes the new last command.
 *
 * Deviation: the decompiler expressed the 5-element walk as a while/goto over the anonymous timer/flag
 * sub-structs; restored to a for loop over the named parameter_flags/parameter_timers arrays. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/observer_command.h"

extern void *memcpy(void *dst, const void *src, unsigned int n);

void observer_update_command(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    observer_command *pending = obs->pending_command;
    int i;

    if ( (pending->flags & (1u << _observer_command_valid_bit)) == 0 )
        return;

    for ( i = 0; i < 5; i++ )
    {
        float last_timer = obs->last_command.parameter_timers[i];

        if ( (pending->___u4.parameter_flags[i] & 1) != 0 )
        {
            if ( (pending->___u4.parameter_flags[i] & 2) == 0
              && pending->parameter_timers[i] < last_timer )
                pending->parameter_timers[i] = last_timer > 2.0f ? 2.0f : last_timer;
        }
        else
        {
            if ( pending->timer >= last_timer || (pending->flags & (1u << _observer_command_force_time_bit)) != 0 )
                pending->parameter_timers[i] = pending->timer;
            else
                pending->parameter_timers[i] = last_timer > 2.0f ? 2.0f : last_timer;
        }
    }

    memcpy(&obs->last_command, obs->pending_command, sizeof(observer_command));
}
