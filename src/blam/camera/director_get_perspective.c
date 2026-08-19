/* director_get_perspective @0x836E4730 — the camera perspective for a local player, derived from the
 * director's active camera procedure: _director_perspective_first_person (once the camera-change pause has
 * elapsed), _director_perspective_third_person (following), _director_perspective_scripted, else
 * _director_perspective_neutral. The resolved value is cached back into the director.
 */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_perspective.h"

#include "headers/following_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/first_person_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/dead_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern void following_camera_update(following_camera *camera, const camera_control *controls, observer_command *result);
extern void scripted_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);

int16_t director_get_perspective(int16_t local_player_index)
{
    director *director = &director_globals.local_players[local_player_index];

    /* attest: (void *) funcptr casts removed — compare the proc pointer directly against each
     * camera-update function (identity dispatch; the decompiler routed the compare through void *) */
    if ( director->camera_proc == first_person_camera_update )
    {
        if ( director->camera_change_pause == 0.0 )
        {
            director->perspective = _director_perspective_first_person;
            return _director_perspective_first_person;
        }
    }
    else if ( director->camera_proc == following_camera_update )
    {
        director->perspective = _director_perspective_third_person;
        return _director_perspective_third_person;
    }
    else
    {
        director->perspective = (director->camera_proc == scripted_camera_update)
                                    ? _director_perspective_scripted
                                    : _director_perspective_neutral;
    }
    /* attest: (unsigned __int16) cast removed — redundant under int16_t return (binary lhz 0x56 + caller extsh) */
    return director->perspective;
}
