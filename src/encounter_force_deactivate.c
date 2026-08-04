#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/blam_data_globals.h"

extern void encounter_deactivate(int encounter_index);

void encounter_force_deactivate(int encounter_index)
{
    DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->remain_active_timer = 0;
    encounter_deactivate(encounter_index);
}
