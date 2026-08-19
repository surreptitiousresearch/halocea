/* ai_handle_enter_vehicle @0x836E9518 — when a unit enters a vehicle, fire an AI communication event
 * (type 36) if the unit is an AI actor (object+500 != -1). vehicle_index is unused. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_data.h"
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

void ai_handle_enter_vehicle(int unit_index, int vehicle_index)
{
    (void)vehicle_index;

    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    if (unit->unit.actor_index != -1)
        ai_communication_event(_ai_communication_vehicle_entry, unit_index, -1, -1, -1, -1, nullptr);
}
