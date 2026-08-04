/* object_hidden_cam_mask @0x83683050 — decide, for HCEX rendering, whether an object should be hidden or
 * treated as a first-person camera object, returning a per-viewport mask. In single-player (or when no coop
 * player is selected) it checks whether the object is a local player's controlled unit and whether that
 * player's director is in first-person; a first-person player's own unit yields the "hide in this viewport"
 * codes (3 single-player, or 1/2 for split-screen viewport 0/1). Otherwise it falls back to
 * scripted_camera_object_is_first_person_camera (3 if the object is a scripted first-person camera, else 0).
 *
 * Player unit index is read at player_data record +52 with the 512-byte salted stride. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t local_player_count(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern uint8_t scripted_camera_object_is_first_person_camera(int object_index);

static int local_player_unit_index(int local_player_index)
{
    int player_index = local_player_get_player_index(local_player_index);
    if ( player_index == -1 )
        return -1;
    return DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
}

int object_hidden_cam_mask(int object_index)
{
    if ( local_player_count() <= 1 )
    {
        if ( local_player_unit_index(0) != object_index )
            return scripted_camera_object_is_first_person_camera(object_index) ? 3 : 0;
        if ( !director_get_perspective(0) )
            return 3;
        return scripted_camera_object_is_first_person_camera(object_index) ? 3 : 0;
    }

    if ( hcex_coop_local_player_index < 0 )
    {
        if ( local_player_unit_index(0) == object_index && !director_get_perspective(0) )
            return 1;
        if ( local_player_unit_index(1) == object_index && !director_get_perspective(1) )
            return 2;
        return scripted_camera_object_is_first_person_camera(object_index) ? 3 : 0;
    }

    int16_t coop_player = hcex_coop_local_player_index;
    if ( local_player_unit_index(coop_player) == object_index && !director_get_perspective(coop_player) )
        return 3;
    return scripted_camera_object_is_first_person_camera(object_index) ? 3 : 0;
}
