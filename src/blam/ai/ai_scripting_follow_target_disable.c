/* ai_scripting_follow_target_disable @0x83772668 — clear an encounter's follow_target_type, so it stops
 * following. */

#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/ai_follow_target_type.h"
#include "headers/blam_data_globals.h"


void ai_scripting_follow_target_disable(int ai_index)
{
    if ( ai_index != -1 )
        DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index)->follow_target_type = _follow_target_none;
}
