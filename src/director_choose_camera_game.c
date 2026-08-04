/* director_choose_camera_game @ 0x836E51B8 — pick the camera for a local player during normal gameplay.
 * On initialize it forces the first-person camera. Otherwise: a key press rotates through the game
 * camera modes (first/following/orbiting); then, unless a scripted camera is active, it chooses the
 * game perspective and swaps in (or out of) the dead/respawn camera based on whether the player is
 * awaiting respawn (no unit, respawn input latched). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/director.h"
#include "headers/first_person_camera.h"
#include "headers/dead_camera.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern const __int16 director_game_camera_modes[3];

extern void first_person_camera_new(first_person_camera *camera);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern int local_player_get_player_index(int16_t local_player_index);
extern void director_rotate_cameras(int16_t local_player_index, const int16_t *cameras, int16_t camera_count);
extern void director_choose_game_perspective(int16_t local_player_index, uint8_t force);
extern void dead_camera_new(dead_camera *camera, int16_t local_player_index, int unit_index);
extern void dead_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);

extern void director_set_camera(int16_t local_player_index, void (__fastcall *camera_proc)(void *camera_data, const camera_control *, observer_command *), uint8_t interpolate);
void director_choose_camera_game(int16_t local_player_index, uint8_t initialize, uint8_t key)
{
    director *dir = &director_globals.local_players[local_player_index];

    /* dir->camera_proc is one generic void(*)(void*,...) slot; each per-kind update fn takes its
     * concrete camera type, so the function-pointer casts here are binary-faithful type-puns. */
    if ( initialize )
    {
        /* DEVIATION: director_set_camera@0x836E48E8 inlined here (zero-xref donor); interpolate folded to 0 (no camera_change_pause write) — collapsed to a call. */
        first_person_camera_new((first_person_camera *)dir->camera_data);
        director_set_camera(local_player_index, (void (__fastcall *)(void *, const camera_control *, observer_command *))first_person_camera_update, 0);
        return;
    }

    /* player datum (512-byte stride) for this local player */
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, local_player_get_player_index(local_player_index));
        /* dead = no current unit AND has died at least once (player+0xAE) */
        unsigned char wants_dead_camera =
            (player->unit_index == -1 && player->statistics.deaths > 0) ? 1 : 0;

        if ( key )
            director_rotate_cameras(local_player_index, director_game_camera_modes, 3);

        if ( !*director_camera_scripted )
        {
            void (__fastcall *camera_proc)(dead_camera *, const camera_control *, observer_command *);

            director_choose_game_perspective(local_player_index, initialize);
            camera_proc = (void (__fastcall *)(dead_camera *, const camera_control *, observer_command *))dir->camera_proc;

            if ( wants_dead_camera )
            {
                if ( camera_proc != dead_camera_update )
                {
                    /* DEVIATION: director_set_camera@0x836E48E8 inlined here (zero-xref donor); interpolate folded to 1 (camera_change_pause=1.0 unconditional) — collapsed to a call. */
                    dead_camera_new((dead_camera *)dir->camera_data, local_player_index, -1);
                    director_set_camera(local_player_index, (void (__fastcall *)(void *, const camera_control *, observer_command *))dead_camera_update, 1);
                }
            }
            else if ( camera_proc == dead_camera_update )
            {
                /* came back alive while on the dead camera: re-pick the live perspective */
                initialize = 1;
                director_choose_game_perspective(local_player_index, initialize);
            }
        }
    }
}
