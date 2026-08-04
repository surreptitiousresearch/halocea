/* actor_unit_control_throw_grenade @0x8371FCC4 — queue a grenade throw on the actor's controlled unit by
 * setting bit 0x2000 in the per-actor unit-control flags (dword at offset 436 within the actor datum). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_throw_grenade(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor->output.control_flags |= (1u << _unit_control_throw_grenade_bit);
}
