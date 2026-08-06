/* director_update @ 0x836E5948 — drive every local player's camera for the frame. For each active
 * local player it gathers raw input (director_update_controls), selects the camera mode
 * (director_choose_camera), then invokes the active camera proc to resolve an observer_command, and
 * finally hands that command to the observer (observer_set_camera).
 *
 * A scripted camera only runs for the primary local player. When a camera change is in progress
 * (camera_change_pause > 0) the pause counts down by dt each frame; a fast cut back into the
 * first-person camera (pause < 0.2 s) snaps the relevant interpolation parameters to their targets
 * immediately rather than blending.
 *
 * Deviation: the decompiler rendered the 104-byte command-buffer zero-fill as a hand-unrolled qword
 * loop and the clamp as inline `fsel`; restored to memset and an explicit max(.., 0). The byte/word
 * pokes into the command buffer (HIBYTE(v15[19]) etc.) are reconstructed as the named
 * parameter_flags/parameter_timers fields they land on. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/observer_command.h"
#include "headers/observer_command_flags.h"
#include "headers/camera_control.h"

#include "headers/first_person_camera.h"
#include "headers/dead_camera.h"
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void *memset(void *dst, int c, unsigned int n);

extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int director_update_controls(int16_t local_player_index, camera_control *controls);
extern void director_choose_camera(int16_t local_player_index, uint8_t initialize, uint8_t key);
extern void observer_set_camera(int16_t local_player_index, observer_command *command);
extern void scripted_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);

/* last camera proc that ran — typed as director.camera_proc so the store below needs no cast */
void (*g_LastCameraUpdateProc)(void *camera_data, const camera_control *controls, observer_command *result);

void director_update(float dt)
{
    int local_player;

    director_globals.dtime = dt;

    for ( local_player = 0; local_player < 2; local_player++ )
    {
        int player_index = local_player_get_player_index((int16_t)local_player);
        director *dir;
        camera_control control;
        observer_command command_buffer;
        uint8_t updated;
        void (*camera_proc)(void *, const camera_control *, observer_command *);
        observer_command *p_command;

        if ( player_index == -1 )
            continue;

        dir = &director_globals.local_players[local_player];
        dir->inhibited_facing = 0;
        dir->inhibited_input = 0;

        updated = director_update_controls(player_index, &control);
        director_choose_camera(player_index, director_globals.initialize_camera, updated);
        director_globals.initialize_camera = 0;

        memset(&command_buffer, 0, sizeof(observer_command));

        camera_proc = dir->camera_proc;
        /* a scripted camera only resolves for the primary local player */
        if ( camera_proc
          && (camera_proc != scripted_camera_update || local_player == local_player_get_next(-1)) )
        {
            dir->camera_proc(dir->camera_data, &control, &command_buffer);
        }
        g_LastCameraUpdateProc = dir->camera_proc;

        if ( (command_buffer.flags & (1 << _observer_command_valid_bit)) != 0 )
        {
            float pause = dir->camera_change_pause;
            if ( pause != 0.0f )
            {
                if ( pause < 0.2f && dir->camera_proc == first_person_camera_update )
                {
                    /* fast cut back to first person: snap the blended params to their targets */
                    dir->camera_change_pause = 0.0f;
                    command_buffer.___u4.parameter_flags[3] = 3;
                    command_buffer.parameter_timers[0] = 0.0f;
                    command_buffer.___u4.parameter_flags[2] = 3;
                    command_buffer.parameter_timers[2] = 0.0f;
                }
                else if ( command_buffer.timer <= pause )
                {
                    command_buffer.timer = dir->camera_change_pause;
                }
                {
                    float remaining = dir->camera_change_pause - dt;
                    dir->camera_change_pause = remaining >= 0.0f ? remaining : 0.0f;
                }
            }
            p_command = &director_globals.local_players[local_player].command;
            memcpy(p_command, &command_buffer, sizeof(observer_command));
        }
        else
        {
            p_command = &director_globals.local_players[local_player].command;
            p_command->flags &= ~(1 << _observer_command_valid_bit);
        }

        observer_set_camera(player_index, p_command);
    }
}
