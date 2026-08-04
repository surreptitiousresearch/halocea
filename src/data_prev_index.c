#include "headers/data_array.h"
#include "headers/datum_index.h"

/* Returns the datum identifier of the previous in-use element before `index`
 * (or before count if index == -1), scanning backward. Each datum's first int16
 * is its salt: 0 means the slot is free. The returned handle packs salt in the
 * high 16 bits and the slot index in the low 16 bits; -1 if none. */
int data_prev_index(data_array *data, int index)
{
    short slot;
    short size;
    short *datum;

    if (index == -1)
        slot = data->count;
    else
        slot = (short)index;

    slot = (short)(slot - 1);
    if (slot < 0 || slot >= data->count)
        return -1;

    size = data->size;
    datum = (short *)((char *)data->data + size * slot);
    while (!*datum)
    {
        datum = (short *)((char *)datum - size);
        slot = (short)(slot - 1);
        if (slot < 0)
            return -1;
    }
    return BUILD_DATUM_INDEX(*datum, slot);
}
