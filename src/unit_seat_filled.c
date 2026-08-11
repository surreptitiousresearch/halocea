/* unit_seat_filled @0x836CABF8 — true if some unit currently occupies the given seat of a parent
 * object. Scans all unit objects (type mask 3) for one whose parent (dword +284) is parent_object_index
 * and whose seat (word +752) matches seat_index. */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"

uint8_t unit_seat_filled(int parent_object_index, int16_t seat_index)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_unit, 0);
    unit_datum *unit = object_iterator_next(&iterator);
    if ( !unit )
        return 0;
    while ( unit->object.parent_object_index != parent_object_index || unit->unit.parent_seat_index != seat_index )
    {
        unit = object_iterator_next(&iterator);
        if ( !unit )
            return 0;
    }
    return 1;
}
