/* reference_list_remove @0x837DFA28 — unlink the reference to a specific datum index from a chain. Walks the
 * chain (next link at +8), comparing the referenced datum index (+4); on a match, deletes that element and
 * splices the chain past it. No-op if the datum is not present. */

#include "headers/data_array.h"
#include "headers/data_reference.h"

extern void datum_delete(data_array *data, int index);

void reference_list_remove(data_array *array, int *first_reference_index, int datum_index)
{
    int *link = first_reference_index;
    if ( *first_reference_index != -1 )
    {
        data_reference *reference;
        while ( 1 )
        {
            /* stride baked to 12 by the compiler; == array->size for this reference-list pool */
            reference = DATA_ARRAY_ELEMENT(array, data_reference, *link);
            if ( reference->datum_index == datum_index )
                break;
            link = &reference->next_reference_index;
            if ( reference->next_reference_index == -1 )
                return;
        }
        datum_delete(array, *link);
        *link = reference->next_reference_index;
    }
}
