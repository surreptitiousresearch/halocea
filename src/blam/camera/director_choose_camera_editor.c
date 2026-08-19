/* director_choose_camera_editor @ 0x836E5320 — pick the free-flying editor camera for a local player.
 * On initialize (or when some other camera proc is active) install a fresh editor/flying camera. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/flying_camera.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/blam_data_globals.h"

extern void editor_camera_new(flying_camera *camera, int16_t local_player_index);
extern void editor_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result);

void director_choose_camera_editor(int16_t local_player_index, uint8_t initialize, uint8_t key)
{
    director *dir = &director_globals.local_players[local_player_index];

    /* editor_camera_update installs into the generic void(*)(void*,...) camera_proc slot; the
     * concrete first-param decl converts implicitly (like director_choose_camera.c), no cast. */
    if ( initialize || dir->camera_proc != editor_camera_update )
    {
        editor_camera_new((flying_camera *)dir->camera_data, local_player_index);
        dir->debug_controls = 0;
        dir->camera_proc = editor_camera_update;
        dir->debug_input_scale = 1.0f;
    }
}
