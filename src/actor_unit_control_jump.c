/* actor_unit_control_jump @0x8371FC24 — queue a jump on the actor's controlled unit by setting the jump bit
 * (0x2) in the per-actor unit-control flags (dword at offset 436 within the 1828-byte actor datum). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_jump(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->output.control_flags |= (1u << _unit_control_jump_bit);
}
