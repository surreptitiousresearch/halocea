/* director_script_camera @0x836E5118 — enable or disable scripted-camera control for both local players.
 * When enabling, each player's camera is switched to the scripted-camera update proc (full debug input
 * scale, no debug controls); when disabling, each player reverts to the normal game perspective. The
 * scripted-camera subsystem is toggled to match. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/blam_data_globals.h"

#include "headers/dead_camera.h"
extern void scripted_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);
extern void scripted_camera_enable(uint8_t enabled);
extern void director_choose_game_perspective(int16_t local_player_index, uint8_t force);

void director_script_camera(uint8_t enabled)
{
    *director_camera_scripted = enabled;

    for ( int local_player = 0; local_player < 2; local_player = (__int16)(local_player + 1) )
    {
        if ( enabled )
        {
            director *player_director = &director_globals.local_players[local_player];
            player_director->debug_input_scale = 1.0f;
            player_director->camera_proc =
                (void (__fastcall *)(void *, const camera_control *, observer_command *))scripted_camera_update;
            player_director->debug_controls = 0;
        }
        else
        {
            director_choose_game_perspective(local_player, 1u);
        }
        scripted_camera_enable(enabled);
    }
}
