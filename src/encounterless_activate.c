/* encounterless_activate @0x8370A8B0 — marks an encounter-less actor active for 90 ticks. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern int actor_set_active(uint16_t actor_index, uint8_t active);

void encounterless_activate(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->meta.encounterless_active_timer = 90;
    actor_set_active(actor_index, 1);
}
