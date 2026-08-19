/* actor_path_has_path @0x837C8708 — whether the actor currently holds a computed path (per-actor flag in
 * the 1828-byte actor datum at offset 1192). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


uint8_t actor_path_has_path(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    return actor->control.path.path.valid;
}
