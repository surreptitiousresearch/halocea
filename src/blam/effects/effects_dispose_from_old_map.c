/* effects_dispose_from_old_map @ 0x836E0D08 — invalidate effect + effect-location pools */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void effects_dispose_from_old_map(void)
{
    data_make_invalid(effect_data);
    data_make_invalid(effect_location_data);
}
