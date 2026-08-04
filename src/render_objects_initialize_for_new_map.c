/* render_objects_initialize_for_new_map @0x836E5C68 — revalidate the cached object render-state pool. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);

void render_objects_initialize_for_new_map(void)
{
    data_make_valid(cached_object_render_states);
}
