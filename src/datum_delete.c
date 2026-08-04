/* datum_delete @ 0x836FB4C0 — free a data_array element by datum index. Clears the slot's
 * identifier, lowers the first-free hint, and—if the last live slot was removed—shrinks the
 * array's count back over any trailing free slots. Decrements the live count.
 *
 * The original asserts (and the decompiler shows a null-deref crash) when the index is invalid;
 * that path is reproduced as a fatal precondition. */

#include "headers/data_array.h"
#include "headers/datum_index.h"

void datum_delete(data_array *data, int index)
{
    __int16 *identifier;

    if ( (__int16)index < 0
      || (__int16)index >= data->count
      || (identifier = (__int16 *)((char *)data->data + data->size * (__int16)index), !*identifier)
      || (DATUM_INDEX_TO_IDENTIFIER(index) && (__int16)DATUM_INDEX_TO_IDENTIFIER(index) != *identifier) )
    {
        identifier = 0;   /* invalid datum index — fatal (matches the original's crash) */
    }

    *identifier = 0;

    if ( (__int16)index < data->first_free_absolute_index )
        data->first_free_absolute_index = index;

    /* if the highest live slot was freed, retract count past any now-free trailing slots */
    if ( (__int16)index + 1 == data->count )
    {
        int size = data->size;
        int count = data->count;
        do
        {
            identifier = (__int16 *)((char *)identifier - size);
            count = (__int16)(count - 1);
            data->count = count;
        }
        while ( count > 0 && !*identifier );
    }

    --data->actual_count;
}
