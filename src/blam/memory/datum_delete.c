/* datum_delete @ 0x836FB4C0 — free a data_array element by datum index. Clears the slot's
 * identifier, lowers the first-free hint, and—if the last live slot was removed—shrinks the
 * array's count back over any trailing free slots. Decrements the live count.
 *
 * The original asserts (and the decompiler shows a null-deref crash) when the index is invalid;
 * that path is reproduced as a fatal precondition. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/datum_index.h"

void datum_delete(data_array *data, int index)
{
    int16_t *identifier;

    if ( (int16_t)index < 0
      || (int16_t)index >= data->count
      || (identifier = (int16_t *)((char *)data->data + data->size * (int16_t)index), !*identifier)
      || (DATUM_INDEX_TO_IDENTIFIER(index) && (int16_t)DATUM_INDEX_TO_IDENTIFIER(index) != *identifier) )
    {
        identifier = 0;   /* invalid datum index — fatal (matches the original's crash) */
    }

    *identifier = 0;

    if ( (int16_t)index < data->first_free_absolute_index )
        data->first_free_absolute_index = index;

    /* if the highest live slot was freed, retract count past any now-free trailing slots */
    if ( (int16_t)index + 1 == data->count )
    {
        int size = data->size;
        int count = data->count;
        do
        {
            identifier = (int16_t *)((char *)identifier - size);
            count = (int16_t)(count - 1);
            data->count = count;
        }
        while ( count > 0 && !*identifier );
    }

    --data->actual_count;
}
