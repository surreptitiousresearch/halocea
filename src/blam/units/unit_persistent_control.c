/* unit_persistent_control @0x836C7028 — arm a unit's persistent control: store the timer and flag bits.
 *
 * FLAG DOMAIN RESOLVED (prior escalation "unit_persistent_control flags" — old absence claim disproved):
 * persistent_control_flags are unit_control_flags bits (see src/headers/unit_control_flags.h,
 * DB enum $DAB55C4E1FF70DE369E731C396390DE5). Confirmed at the consumer: unit_update.c tests
 * unit.persistent_control_flags & (1u << _unit_control_weapon_primary_trigger_bit), and the producer
 * actor_died.c passes 0x800 == (1u << _unit_control_weapon_primary_trigger_bit). This function is a plain
 * passthrough setter (no literals of its own). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


void unit_persistent_control(int unit_index, int persistent_control_timer, unsigned int persistent_control_flags)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit->unit.persistent_control_timer = persistent_control_timer;
    unit->unit.persistent_control_flags = persistent_control_flags;
}
