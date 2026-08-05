/* director_choose_camera @ 0x836E5728 — select the camera proc for a local player based on the global
 * director game mode:
 *   mode 0 / 1 — gameplay: delegate to director_choose_camera_game (in-game camera rotation)
 *   mode 2     — editor: install the free-flying editor camera
 *   mode 3     — (no camera handling)
 *   mode 4     — script/first-person: install first-person on initialize, else rotate script cameras
 *
 * Deviation: the decompiler's nested game_mode comparisons (`game_mode == 1 && game_mode`,
 * `game_mode != 3 || !game_mode`) are tautological encodings of an equality dispatch; restored to a
 * direct mode switch. The camera_proc casts to flying_camera/first_person_camera are just the opaque
 * camera_data blob reinterpreted per camera type. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_game_mode.h"

typedef struct flying_camera flying_camera;
typedef struct first_person_camera first_person_camera;

#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
extern void director_choose_camera_game(int16_t local_player_index, uint8_t initialize, uint8_t key);
extern void editor_camera_new(flying_camera *camera, int16_t local_player_index);
extern void editor_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result);
extern void first_person_camera_new(first_person_camera *camera);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern void director_rotate_cameras(int16_t local_player_index, const int16_t *cameras, int16_t camera_count);
extern const int16_t director_script_camera_record_camera_modes[];

void director_choose_camera(int16_t local_player_index, uint8_t initialize, uint8_t key)
{
    director *dir = &director_globals.local_players[local_player_index];

    switch ( director_globals.game_mode )
    {
        case _director_game_mode_gameplay:
        case _director_game_mode_gameplay_alt:
            director_choose_camera_game(local_player_index, initialize, key);
            return;

        case _director_game_mode_editor:
            if ( initialize || dir->camera_proc != editor_camera_update )
            {
                editor_camera_new((flying_camera *)dir->camera_data, local_player_index);
                dir->debug_controls = 0;
                dir->camera_proc = editor_camera_update;
                dir->debug_input_scale = 1.0f;
            }
            return;

        case _director_game_mode_scripted:
            if ( initialize )
            {
                first_person_camera_new((first_person_camera *)dir->camera_data);
                dir->debug_input_scale = 1.0f;
                dir->camera_proc = first_person_camera_update;
                dir->debug_controls = 0;
            }
            else if ( key )
            {
                director_rotate_cameras(local_player_index, director_script_camera_record_camera_modes, 4);
            }
            return;

        default:   /* mode 3: no camera handling */
            return;
    }
}
