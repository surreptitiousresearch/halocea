/* director_is_first_person_player @0x836E4568 — true if the first valid local player's camera is the
 * first-person camera. Scans local player slots 0..1 for one with a valid player index.
 *
 * DEVIATION: the decompiler emits a dead "if (!&local_players[idx])" null check (the address of an
 * array element is never null); omitted. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/first_person_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"

#include "headers/first_person_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
extern int local_player_get_player_index(int16_t local_player_index);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);

int director_is_first_person_player(void)
{
    short local_player_index = 0;
    while ( local_player_get_player_index(local_player_index) == -1 )
    {
        if ( ++local_player_index >= 2 )
            return 0;
    }
    if ( (void *)director_globals.local_players[local_player_index].camera_proc != (void *)first_person_camera_update )
        return 0;
    return 1;
}
