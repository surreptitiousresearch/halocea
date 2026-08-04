/* ctf_weapon_drop @0x83806CE8 — clear the dropped weapon's owning object reference
 * (object_datum.object.owner_player_index at datum offset 192). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


void ctf_weapon_drop(int weapon_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    object->object.owner_player_index = -1;
}
