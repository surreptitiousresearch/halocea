/* ai_scripting_follow_target_unit @0x837726C8 — make an encounter follow a specific unit. Sets the
 * follow-target type to 2 (follow unit) and stores the unit index; a unit of -1 disables following. */

#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/ai_follow_target_type.h"
#include "headers/blam_data_globals.h"


void ai_scripting_follow_target_unit(int ai_index, int unit_index)
{
    if ( ai_index == -1 )
        return;

    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index);
    if ( unit_index == -1 )
    {
        encounter->follow_target_type = _follow_target_none;
    }
    else
    {
        encounter->___u43.follow_target_unit_index = unit_index;
        encounter->follow_target_type = _follow_target_unit;
    }
}
