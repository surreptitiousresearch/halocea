/* actor_unit_control_secondary_trigger @0x8371FC94 — set/clear the secondary-fire bit (0x1000) in the
 * actor's unit-control flags (dword at offset 436 within the actor datum). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_secondary_trigger(uint16_t actor_index, uint8_t trigger)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->output.control_flags = trigger ? (actor->output.control_flags | (1u << _unit_control_weapon_secondary_trigger_bit)) : (actor->output.control_flags & ~(1u << _unit_control_weapon_secondary_trigger_bit));
}
