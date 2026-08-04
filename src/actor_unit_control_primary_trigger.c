/* actor_unit_control_primary_trigger @0x8371FC54 — set/clear the primary-fire bit (0x800, bit 0xB) in the
 * actor's unit-control flags and store the analog trigger value (float at offset 456 within the actor datum).
 * DEVIATION: analog_value typed double by the decompiler (soft-float ABI); it is a float. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_primary_trigger(uint16_t actor_index, uint8_t trigger,
                                        float analog_value)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned int flags = actor->output.control_flags;
    actor->output.analog_primary_trigger = analog_value;
    actor->output.control_flags = trigger ? (flags | (1u << _unit_control_weapon_primary_trigger_bit))
                                          : (flags & ~(1u << _unit_control_weapon_primary_trigger_bit));
}
