/* props_dispose_from_old_map @0x837D1758 — per-map teardown: invalidate the prop data pool. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void props_dispose_from_old_map(void)
{
    data_make_invalid(prop_data);
}
