/* actor_clear_output @0x8371EC40 — reset an actor's communication/animation output state: clears a request
 * flag and a scalar (+1744/+1824), shifts a 9-dword block (+372..404) into a history slot one struct-width
 * further out (+1788..1820, following the same "current" value copied to +1788), zeroes a direction vector
 * (+1760), and marks a cached index invalid (+1772).
 *
 * Fields resolved to named actor_datum members (communication/animation output block; see actor_datum.h). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


void actor_clear_output(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    actor->output.control_flags = 0;
    actor->output.analog_primary_trigger = 0.0f;

    actor->output.facing_vector  = actor->input.facing_vector;
    actor->output.aiming_vector  = actor->input.aiming_vector;
    actor->output.looking_vector = actor->input.looking_vector;

    actor->output.throttle = *global_zero_vector3d;
    actor->output.animation.impulse = -1;
}
