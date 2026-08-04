/* object_is_first_person_camera 0x836E5CB0 — true if the given object is currently viewed through a
 * first-person camera: either it is the local player's own unit while in first-person perspective, or a
 * cutscene script has designated it the first-person camera object. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t director_get_perspective(int16_t local_player_index);
extern uint8_t scripted_camera_object_is_first_person_camera(int object_index);

uint8_t object_is_first_person_camera(int object_index)
{
    int unit_index;
    if ( local_player_get_player_index(render.local_player_index) == -1 )
        unit_index = -1;
    else
        unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                   local_player_get_player_index(render.local_player_index))->unit_index;

    if ( unit_index == object_index && !director_get_perspective(render.local_player_index) )
        return 1;
    if ( scripted_camera_object_is_first_person_camera(object_index) )
        return 1;
    return 0;
}
