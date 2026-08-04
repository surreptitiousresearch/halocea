/* object_set_ranged_attack_inhibited @0x836B1880 — set or clear an object's ranged-attack inhibition flag
 * (object data +0x106 bit 0x100). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


void object_set_ranged_attack_inhibited(int object_index, uint8_t inhibited)
{
    if ( object_index == -1 )
        return;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if ( inhibited )
        object_data->object.damage_flags |= (1u << _object_ranged_attack_inhibited_bit);
    else
        object_data->object.damage_flags &= ~(1u << _object_ranged_attack_inhibited_bit);
}
