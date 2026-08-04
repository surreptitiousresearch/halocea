/* effects_initialize_for_new_map @0x836E0CD0 — reset the effect and effect-location pools. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void effects_initialize_for_new_map(void)
{
    data_make_valid(effect_data);
    data_make_valid(effect_location_data);
}
