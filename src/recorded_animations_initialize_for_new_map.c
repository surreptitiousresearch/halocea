/* recorded_animations_initialize_for_new_map @0x83712348 — reset the recorded-animation thread pool. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void recorded_animations_initialize_for_new_map(void)
{
    data_make_valid(animation_threads);
}
