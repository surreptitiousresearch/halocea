/* observer_set_camera @ 0x8370EA50 — hand a freshly-built camera command to a local player's observer.
 * The very first command for an observer is forced to snap instantly (flag 0x8 set, all interpolation
 * timers zeroed) so the camera does not blend in from an undefined previous state. */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"

void observer_set_camera(int16_t local_player_index, observer_command *command)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    int was_initialized = obs->first_command;

    obs->updated_for_frame = 0;
    obs->pending_command = command;

    if ( !was_initialized )
    {
        observer_command *pending;
        obs->first_command = 1;
        command->timer = 0.0f;
        obs->pending_command->flags |= (1u << _observer_command_force_time_bit);   /* force instant cut */
        pending = obs->pending_command;
        pending->position_timer     = 0.0f;
        pending->focus_offset_timer = 0.0f;
        pending->distance_timer     = 0.0f;
        pending->field_of_view_timer = 0.0f;
        pending->orientation_timer  = 0.0f;
    }
}
