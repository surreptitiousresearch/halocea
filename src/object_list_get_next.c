/* object_list_get_next @0x83775660 — continue iterating an object list. *reference_index is the cursor left by
 * object_list_get_first / a prior object_list_get_next; returns the next object index (or -1 at end) and advances
 * the cursor. The object_list_index parameter is unused (the cursor alone walks the chain). Reference elements
 * are data_reference datums in the shared object_list_data pool. */

#include "headers/data_array.h"
#include "headers/data_reference.h"
#include "headers/blam_data_globals.h"


int object_list_get_next(int object_list_index, int *reference_index)
{
    if ( *reference_index == -1 )
        return -1;

    /* recovered: raw 4*(u16)i + 4*((2i)&0x1FFFE) is 12*(u16)i, i.e. the data_reference element index */
    data_reference *reference = &((data_reference *)object_list_data->data)[(unsigned short)*reference_index];
    *reference_index = reference->next_reference_index;   /* advance cursor */
    return reference->datum_index;                        /* object index */
}
