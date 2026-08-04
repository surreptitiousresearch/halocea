/* director_is_first_person @0x836E4468 — true when a local player's camera director is running the
 * first-person camera and is not currently blending out of a camera change (camera_change_pause <= 0). */

#include <stdint.h>
#include "headers/director.h"

#include "headers/first_person_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);

uint8_t director_is_first_person(int16_t local_player_index)
{
    director *dir = &director_globals.local_players[local_player_index];
    /* identity test against a generic camera-update slot (camera_proc is void*-typed): void* pun is faithful */
    if ( (void *)dir->camera_proc == (void *)first_person_camera_update )
        return dir->camera_change_pause <= 0.0f;
    return 0;
}
