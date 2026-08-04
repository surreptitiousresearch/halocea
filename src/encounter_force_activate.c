/* encounter_force_activate @0x8370D428 — activate an encounter and set its remain_active_timer to 150
 * ticks, keeping it active for a while even without the usual triggers. Returns encounter_activate's result. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_activate(int encounter_index);

void encounter_force_activate(int encounter_index)
{
    DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->remain_active_timer = 150;
    encounter_activate(encounter_index);
}
