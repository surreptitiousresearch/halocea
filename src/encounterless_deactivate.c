/* encounterless_deactivate @0x8370A8D8 — clears an encounter-less actor's active timer and deactivates it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern int actor_set_active(uint16_t actor_index, uint8_t active);

void encounterless_deactivate(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->meta.encounterless_active_timer = 0;
    actor_set_active(actor_index, 0);
}
