/* object_list_gc @0x83775718 — garbage-collect object lists: walk every valid list header and delete those
 * whose reference_count is zero (an unreferenced/transient marker), freeing the reference chain and the
 * header datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_list_header_datum.h"
#include "headers/blam_data_globals.h"

extern int data_next_index(const data_array *data, int16_t index);
extern void reference_list_delete(data_array *array, int first_reference_index);
extern void datum_delete(data_array *data, int index);

void object_list_gc(void)
{
    /* disasm: tail b __restgprlr_29; r3 at exit is data_next_index residue (-1), 0 consumers => void */
    int index = data_next_index(object_list_header_data, -1);
    while ( index != -1 )
    {
        data_array *array = object_list_header_data;
        object_list_header_datum *list =
            DATA_ARRAY_ELEMENT(object_list_header_data, object_list_header_datum, index);
        if ( !list->reference_count && index != -1 )
        {
            reference_list_delete(object_list_data, list->first_reference_index);
            datum_delete(object_list_header_data, index);
            array = object_list_header_data;
        }
        index = data_next_index(array, index);
    }
}
