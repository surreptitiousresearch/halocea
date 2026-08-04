/* props_initialize_for_new_map @0x837D1750 — revalidate the AI prop datum array for a new map. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void props_initialize_for_new_map(void)
{
    data_make_valid(prop_data);
}
