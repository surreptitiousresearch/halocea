/* ai_scripting_follow_distance @0x83772758 — set the follow-target stand-off distance for an encounter. */

#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"


void ai_scripting_follow_distance(int ai_index, float follow_target_distance)
{
    if ( ai_index != -1 )
        DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index)->follow_target_distance = follow_target_distance;
}
