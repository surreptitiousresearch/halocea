/* reference_list_add @0x83775358 — push a datum index onto the head of a reference chain. Allocates a 12-byte
 * reference element (referenced datum index at +4, next reference index at +8) and updates the caller's chain
 * head. No-op on a full pool. */

#include "headers/data_array.h"
#include "headers/data_reference.h"

extern int datum_new(data_array *data);

void reference_list_add(data_array *array, int *first_reference_index, int datum_index)
{
    int reference_index = datum_new(array);
    if ( reference_index != -1 )
    {
        /* recovered: raw 4*(u16)i + 4*((2i)&0x1FFFE) is 12*(u16)i, i.e. the data_reference element index */
        data_reference *reference = &((data_reference *)array->data)[(unsigned short)reference_index];
        reference->datum_index = datum_index;
        reference->next_reference_index = *first_reference_index;
        *first_reference_index = reference_index;
    }
}
