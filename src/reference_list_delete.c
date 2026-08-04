/* reference_list_delete @0x837753B0 — free an entire reference chain, walking next-reference links (+8) and
 * deleting each element from the pool. */

#include "headers/data_array.h"
#include "headers/data_reference.h"

extern void datum_delete(data_array *data, int index);

void reference_list_delete(data_array *array, int first_reference_index)
{
    while ( first_reference_index != -1 )
    {
        const data_reference *reference =
            &((const data_reference *)array->data)[(unsigned short)first_reference_index];
        datum_delete(array, first_reference_index);
        first_reference_index = reference->next_reference_index;
    }
}
