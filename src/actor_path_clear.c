/* actor_path_clear @0x837C86D0 — resets the actor's pathfinding progress fraction and two associated
 * status bytes (float offset 296, byte offsets 1192/1156 — inside actor_datum's action_data union, no DB
 * struct name established for these fields). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_path_clear(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->control.path.destination_original_distance = 0.0f;
    actor->control.path.path.valid = 0;
    actor->control.path.at_destination = 1;
}
