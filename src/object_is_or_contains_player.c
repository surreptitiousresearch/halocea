/* object_is_or_contains_player @0x837F7BCC — true if the object is a player's unit, or transitively
 * contains/parents a player. It checks: the object itself, each child object (recursively), the parent
 * chain, and finally whether the object is an item type (type bits 2-4: weapon/equipment/garbage)
 * whose item flags bit 0x2 marks it as held by a player. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/item_datum.h"
#include "headers/item_flags.h"
#include "headers/blam_data_globals.h"

extern int player_index_from_unit_index(int unit_index);

static object_datum *object_data(int object_index)
{
    return DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
}

uint8_t object_is_or_contains_player(int object_index)
{
    object_datum *object = object_data(object_index);
    int found = player_index_from_unit_index(object_index) != -1;

    if (!found)
    {
        for (int child = object->object.first_child_object_index; child != -1; )
        {
            object_datum *child_object = object_data(child);
            if (object_is_or_contains_player(child))
            {
                found = 1;
                break;
            }
            child = child_object->object.next_object_index;
        }
    }

    if (!found)
    {
        for (int parent = object->object.parent_object_index; parent != -1; )
        {
            object_datum *parent_object = object_data(parent);
            if (player_index_from_unit_index(parent) != -1)
            {
                found = 1;
                break;
            }
            parent = parent_object->object.parent_object_index;
        }
    }

    if (found)
        return found;

    if (((1 << (unsigned __int16)object->object.type) & 0x1C) == 0)
        return 0;
    if ((((item_datum *)object)->item.flags & (1u << _item_belongs_to_player_bit)) == 0)
        return 0;
    return 1;
}
