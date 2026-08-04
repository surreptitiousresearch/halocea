/* encounter_pursuit_position_already_examined @0x8370D360 — has a firing position already been examined
 * during the current pursuit? Looks up the pursuit record for (encounter, firing position) since
 * history_start_time; if found, it counts as examined when either 7+ actors have already examined it
 * (examiner list full) or this actor is among the up-to-6 recorded examiners (40-byte pursuit records:
 * last-examined time dword +4, examiner count word +8, examiner actor indices dwords +12..+32). The
 * count/time are reported through the optional references even on a miss. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/pursuit_datum.h"
#include "headers/blam_data_globals.h"


extern int encounter_find_pursuit(uint16_t encounter_index, int16_t firing_position_index, int history_start_time, uint8_t force_create);

uint8_t encounter_pursuit_position_already_examined(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time, int16_t *actor_count_reference, int *last_examined_time_reference)
{
    int pursuit_index = encounter_find_pursuit(encounter_index, firing_position_index, history_start_time, 0);
    __int16 actor_count = 0;
    int last_examined_time = -1;
    unsigned __int8 examined = 0;

    if ( pursuit_index != -1 )
    {
        pursuit_datum *pursuit = DATA_ARRAY_ELEMENT(pursuit_data, pursuit_datum, pursuit_index);
        actor_count = pursuit->actor_count;
        last_examined_time = pursuit->last_examined_time;

        if ( actor_count >= 7 )
        {
            examined = 1;
        }
        else
        {
            for ( __int16 i = 0; i < 6; i++ )
            {
                if ( pursuit->actor_indices[i] == actor_index )
                {
                    examined = 1;
                    break;
                }
            }
        }
    }

    if ( actor_count_reference )
        *actor_count_reference = actor_count;
    if ( last_examined_time_reference )
        *last_examined_time_reference = last_examined_time;
    return examined;
}
