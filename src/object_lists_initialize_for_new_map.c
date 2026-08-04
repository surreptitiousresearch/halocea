/* object_lists_initialize_for_new_map @ 0x83775488 — validate the object-list pools */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void object_lists_initialize_for_new_map(void)
{
    data_make_valid(object_list_header_data);
    data_make_valid(object_list_data);
}
