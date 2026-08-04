/* actor_stimulus_was_surprised @ 0x837D4E04 — flag that this actor was surprised
 * (stimuli.was_surprised, actor+752). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_was_surprised(uint16_t actor_index)
{
    /* recovered: *((_BYTE *)actor_data->data + 1828*idx + 752) -> stimuli.was_surprised */
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->stimuli.was_surprised = 1;
}
