/* reference_list_copy @0x837DFAB0 — overwrite a destination reference pool with a source pool element-for-
 * element (12-byte elements). Where the source slot is occupied (datum header halfword at +0 non-zero) its three
 * words are copied; where the source slot is empty but the destination slot is occupied, the destination datum
 * is deleted. Both pools must share the same maximum_count. */

#include "headers/data_array.h"
#include "headers/data_reference.h"

extern void datum_delete(data_array *data, int index);

void reference_list_copy(data_array *result, const data_array *source)
{
    const data_reference *source_element = (const data_reference *)source->data;
    data_reference *result_element = (data_reference *)result->data;
    if ( result->maximum_count > 0 )
    {
        int index = 0;
        do
        {
            if ( source_element->identifier )
            {
                *result_element = *source_element;
            }
            else if ( result_element->identifier )
            {
                datum_delete(result, index);
            }
            ++source_element;
            index = (short)(index + 1);
            ++result_element;
        }
        while ( index < result->maximum_count );
    }
}
