/* object_lists_dispose_from_old_map @0x837754C0 — invalidate the per-map object-list datum arrays. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void object_lists_dispose_from_old_map(void)
{
    data_make_invalid(object_list_header_data);
    data_make_invalid(object_list_data);
}
