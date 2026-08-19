/* datum_get @0x836FAFF8 */
#include "headers/data_array.h"
#include <stddef.h>
#include "headers/datum_index.h"

/* datum_get — resolve a datum_index to the address of its element in `data`, or NULL if the slot is
 * out of range, unoccupied, or the caller's identifier salt doesn't match the one stored in the slot.
 *
 * A datum_index packs the absolute slot in its low 16 bits and an identifier salt in its high 16 bits.
 * Bit 0x8000 of the low word marks an invalid/sentinel index. The first 16-bit word of every element
 * holds that element's own identifier; a zero identifier means the slot is free. When the caller
 * supplies a nonzero salt it must equal the stored identifier. */
void *datum_get(data_array *data, int index)
{
    char *element;
    int byte_offset;
    short datum_identifier;

    if ((index & 0x8000u) != 0)
        return NULL;
    if ((short)index >= data->count)
        return NULL;

    element = (char *)data->data;
    byte_offset = data->size * (short)index;
    datum_identifier = *(short *)&element[byte_offset];

    /* datum handle packs identifier (salt) in high 16 bits; high-word value semantics (not BE mem-order) */
    if (!datum_identifier || (DATUM_INDEX_TO_IDENTIFIER(index) && (short)DATUM_INDEX_TO_IDENTIFIER(index) != datum_identifier))
        return NULL;

    return &element[byte_offset];
}
