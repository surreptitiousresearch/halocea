/* director_choose_camera_script_camera_record @ 0x836E53A8 — camera selection while recording a
 * scripted camera path. On initialize install the first-person camera; otherwise a key press rotates
 * through the script-camera-record camera modes. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/first_person_camera.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/blam_data_globals.h"

extern const __int16 director_script_camera_record_camera_modes[];

extern void first_person_camera_new(first_person_camera *camera);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern void director_rotate_cameras(int16_t local_player_index, const int16_t *cameras, int16_t camera_count);

void director_choose_camera_script_camera_record(int16_t local_player_index, uint8_t initialize, uint8_t key)
{
    if ( initialize )
    {
        director *dir = &director_globals.local_players[local_player_index];
        first_person_camera_new((first_person_camera *)dir->camera_data);
        dir->debug_input_scale = 1.0f;
        dir->camera_proc = first_person_camera_update;
        dir->debug_controls = 0;
    }
    else if ( key )
    {
        director_rotate_cameras(local_player_index, director_script_camera_record_camera_modes, 4);
    }
}
