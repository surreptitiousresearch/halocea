/* datum_new_at_plain_index_hack_for_player_data @0x836FB344 — allocate a data_array element at a specific
 * (plain) index rather than the next free slot, used so player data lands at a network-stable index.
 * Fails if the index is out of range or already occupied. Grows the array's high-water count as needed and
 * returns the packed datum id (identifier in the high word, index in the low word), or -1 on failure. */

#include "headers/data_array.h"
#include "headers/datum_header.h"
#include "headers/datum_index.h"

extern void datum_initialize(data_array *data, datum_header *header);

int datum_new_at_plain_index_hack_for_player_data(data_array *data, int index)
{
    if (index < 0 || index >= data->maximum_count)
        return -1;

    datum_header *datum = (datum_header *)((char *)data->data + data->size * index);
    if (datum->identifier)
        return -1;

    int high_water = data->count;
    ++data->actual_count;
    if (index >= high_water)
        data->count = index + 1;

    datum_initialize(data, datum);
    return BUILD_DATUM_INDEX(datum->identifier, index);
}
