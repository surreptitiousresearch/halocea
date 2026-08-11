/* dangerous_items_near_player @0x83757C48 — true if any "dangerous" item object (type mask 0x1C,
 * connected-objects flag set) currently has a positive danger value (short @ object word +252). */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_type.h"
#include "headers/item_datum.h"

uint8_t dangerous_items_near_player(void)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_item, 1u);
    item_datum *object = object_iterator_next(&iterator);
    if ( !object )
        return 0;
    while ( object->item.detonation_ticks <= 0 )  /* item+504 */
    {
        object = object_iterator_next(&iterator);
        if ( !object )
            return 0;
    }
    return 1;
}
