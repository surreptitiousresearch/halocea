/* objects_disconnect_from_structure_bsp @0x836EF8C8 — run when the active structure BSP is torn down.
 * Walks every live object; for each object still flagged connected-to-map (_object_connected_to_map_bit,
 * 0x800) with no parent, disconnects it from the map, then re-stamps the connected flag (so the matching
 * objects_reconnect_to_structure_bsp pass knows to re-place it). Every object then has its type-specific
 * structure-BSP state torn down via object_type_disconnect_from_structure_bsp.
 *
 * DEVIATION: the re-set of 0x800 after object_disconnect_from_map is faithful to the shipped code — the
 * disconnect clears the flag internally and this restores it for the reconnect pass. */

#include "headers/object_iterator.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"

extern void *object_iterator_next(object_iterator *iterator);
extern void object_disconnect_from_map(int object_index);
extern void object_type_disconnect_from_structure_bsp(int object_index);

void objects_disconnect_from_structure_bsp(void)
{
    object_iterator iterator;
    iterator.flags = 0;
    iterator.signature = -2038004090;
    iterator.type_flags = -1;
    iterator.absolute_index = 0;
    iterator.index = -1;

    object_datum *object;
    for ( object = object_iterator_next(&iterator); object;
          object = object_iterator_next(&iterator) )
    {
        int index = iterator.index;
        if ( (object->object.flags & (1u << _object_connected_to_map_bit)) != 0
          && object->object.parent_object_index == -1 )
        {
            object_disconnect_from_map(iterator.index);
            object->object.flags |= (1u << _object_connected_to_map_bit);
        }
        object_type_disconnect_from_structure_bsp(index);
    }
}
