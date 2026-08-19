/* render_objects_dispose_from_old_map @0x836E5C78 — per-map teardown: invalidate the cached per-object
 * render-state pool if it is live. */

#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void render_objects_dispose_from_old_map(void)
{
    if ( cached_object_render_states && cached_object_render_states->valid )
        data_make_invalid(cached_object_render_states);
}
