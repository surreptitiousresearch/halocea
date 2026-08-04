/* actor_unit_control_exact_facing @0x8371FCF4 — set or clear the actor's "exact facing" unit-control
 * flag (bit 0x20 of the flags dword at actor record index 436). Actor records are stride 1828. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


void actor_unit_control_exact_facing(uint16_t actor_index, uint8_t exact_facing)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned int flags = actor->output.control_flags;
    if ( exact_facing )
        flags |= (1u << _unit_control_exact_facing_bit);
    else
        flags &= ~(1u << _unit_control_exact_facing_bit);
    actor->output.control_flags = flags;
}
