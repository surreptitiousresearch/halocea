/* weapon_get_effect_object_index @0x836D96D8 — when the weapon object is flagged as an attachment (object
 * flags bit 0x1) and has a parent, effects originate from the parent; return it. Otherwise the weapon is its
 * own effect source. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


int weapon_get_effect_object_index(int weapon_index)
{
    object_datum *weapon = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    if ( (weapon->object.flags & (1u << _object_invisible_bit)) != 0 )
    {
        int parent = weapon->object.parent_object_index;
        if ( parent != -1 )
            return parent;
    }
    return weapon_index;
}
