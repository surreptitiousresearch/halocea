/* unit_kill_no_statistics @0x836C6C10 — flag a unit to be killed without recording a death
 * statistic (object damage_flags bit 0x2000). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


void unit_kill_no_statistics(int unit_index)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit->object.damage_flags |= (1u << _object_die_act_of_god_no_statistics_bit);
}
