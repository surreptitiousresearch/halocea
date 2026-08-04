/* player_control_camera_control_is_active @0x836DE494 — true when the player may steer the camera:
 * not while the control-disable flag is set, and not while the game clock is paused. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_camera_off_flags.h"

extern uint8_t game_time_get_paused(void);

uint8_t player_control_camera_control_is_active(void)
{
    if ( (player_control_globals->control_flags & (1u << _player_control_camera_off_bit)) != 0 )
        return 0;
    if ( game_time_get_paused() )
        return 0;
    return 1;
}
