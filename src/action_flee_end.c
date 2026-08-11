/* action_flee_end @0x83825130 — stops the actor's unit from running blindly when it has one attached. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern void unit_stop_running_blindly(int unit_index);

void action_flee_end(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if (actor->meta.unit_index != -1)
        unit_stop_running_blindly(actor->meta.unit_index);
}
