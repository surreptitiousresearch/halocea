/* ai_scripting_follow_target_ai @0x83772710 — make an encounter follow another AI (encounter). Sets the
 * follow-target type to 3 (follow AI) and stores the target AI index; a target of -1 disables following. */

#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/ai_follow_target_type.h"
#include "headers/blam_data_globals.h"


void ai_scripting_follow_target_ai(int ai_index, int target_ai_index)
{
    if ( ai_index == -1 )
        return;

    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index);
    if ( target_ai_index == -1 )
    {
        encounter->follow_target_type = _follow_target_none;
    }
    else
    {
        encounter->___u43.follow_target_ai_index = target_ai_index;
        encounter->follow_target_type = _follow_target_ai;
    }
}
