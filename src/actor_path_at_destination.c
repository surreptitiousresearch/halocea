/* actor_path_at_destination @0x837C8728 — reports whether the actor has finished following its current
 * path. An actor with no valid path result is trivially "at destination"; otherwise it is at the
 * destination only when its path-control at_destination flag is set. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


uint8_t actor_path_at_destination(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (!actor->control.path.path.valid)
        return 1;
    if (actor->control.path.at_destination != 0)
        return 1;
    return 0;
}
