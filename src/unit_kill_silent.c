/* unit_kill_silent @0x836C6BD8 — flag a unit to be killed silently, with no death effects/animation (object
 * data +0x106 bit 0x40). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


void unit_kill_silent(int unit_index)
{
    object_datum *object_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    object_data->object.damage_flags |= (1u << _object_die_act_of_god_silent_bit);
}
