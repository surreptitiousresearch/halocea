/* actor_unit_control_crouch @0x8371FBF4 — set or clear the crouch-modifier bit (0x1, bit 0) in the actor's
 * unit-control flags (dword at offset 436 within the actor datum). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_crouch(uint16_t actor_index, uint8_t crouch)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned int flags = actor->output.control_flags;
    actor->output.control_flags = crouch ? (flags | (1u << _unit_control_crouch_modifier_bit))
                                         : (flags & ~(1u << _unit_control_crouch_modifier_bit));
}
