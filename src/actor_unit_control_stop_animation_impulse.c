/* actor_unit_control_stop_animation_impulse @0x8371FD2C — clear an actor's pending unit-control
 * animation impulse (output.animation.impulse set to -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_stop_animation_impulse(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->output.animation.impulse = -1;
}
