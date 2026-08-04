/* reference_list_get_next_datum_index @0x83775318 — iterate a reference chain. *reference_index is the cursor;
 * returns the referenced datum index (+4) at the cursor and advances the cursor to the next link (+8), or -1 at
 * the end. */

#include "headers/data_array.h"
#include "headers/data_reference.h"

int reference_list_get_next_datum_index(data_array *array, int *reference_index)
{
    if ( *reference_index == -1 )
        return -1;

    /* recovered: raw 4*(u16)i + 4*((2i)&0x1FFFE) is 12*(u16)i, i.e. the data_reference element index */
    data_reference *reference = &((data_reference *)array->data)[(unsigned short)*reference_index];
    *reference_index = reference->next_reference_index;
    return reference->datum_index;
}
