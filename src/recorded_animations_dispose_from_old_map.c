/* recorded_animations_dispose_from_old_map @ 0x83711FB0 — invalidate animation pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
void recorded_animations_dispose_from_old_map(void)
{
    data_make_invalid(animation_threads);
}
