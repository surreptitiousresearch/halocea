/* ai_handle_killing_spree @0x836E8A68 — raise an AI communication event when a unit reaches a kill streak.
 * The threshold is 5 kills for a player-controlled unit, 3 for an AI unit (object_data+536 == -1 means no
 * controlling player). Returns 1 (and fires the event) when the streak count meets the threshold. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

uint8_t ai_handle_killing_spree(int unit_index, int16_t killing_spree_count)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int16_t threshold = unit->unit.player_index == -1 ? 3 : 5;
    if ( killing_spree_count >= threshold )
    {
        ai_communication_event(_ai_communication_killing_spree, unit_index, -1, -1, -1, -1, 0);
        return 1;
    }
    return 0;
}
