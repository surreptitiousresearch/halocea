/* datum_new_at_index @ 0x836FB288 — claim a specific slot in a data_array by datum id.
 * Returns the full datum identifier ((salt<<16)|index) or -1 on failure. */

#include "headers/data_array.h"
#include "headers/datum_header.h"
#include "headers/datum_index.h"

extern void datum_initialize(data_array *data, datum_header *header);

int datum_new_at_index(data_array *data, int index)
{
    short slot = (short)index;
    short salt = (short)DATUM_INDEX_TO_IDENTIFIER(index);
    datum_header *datum;
    int count;

    if ( (__int16)index < 0 )               return -1;
    if ( slot >= data->maximum_count )      return -1;
    if ( !salt )                            return -1;

    datum = (datum_header *)((char *)data->data + data->size * slot);
    if ( datum->identifier )                return -1;

    count = data->count;
    ++data->actual_count;
    if ( slot >= count )
        data->count = slot + 1;  /* disasm 836FB300: addi of the sign-extended slot, not the full handle */
    datum_initialize(data, datum);
    datum->identifier = salt;
    return BUILD_DATUM_INDEX(salt, slot);
}
