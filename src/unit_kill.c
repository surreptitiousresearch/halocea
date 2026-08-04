/* unit_kill @0x836C6BA0 — flag a unit to be killed (object damage_flags bit 0x20: scripted death). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


void unit_kill(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit->object.damage_flags |= (1u << _object_die_act_of_god_bit);
}
