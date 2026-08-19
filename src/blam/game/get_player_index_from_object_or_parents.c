/* get_player_index_from_object_or_parents @0x836B1598 — walk up the object's parent chain until a unit
 * (type mask bit for unit) is found, then return the player controlling it, or -1. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int player_index_from_unit_index(int unit_index);

int get_player_index_from_object_or_parents(int object_index)
{
    int current = object_index;
    if ( object_index == -1 )
        return -1;

    while ( !object_try_and_get_and_verify_type(current, object_mask_unit) )
    {
        object_datum *object_data =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current)->datum;
        current = object_data->object.parent_object_index;
        if ( current == -1 )
            return -1;
    }
    return player_index_from_unit_index(current);
}
