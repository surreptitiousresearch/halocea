/* decals_initialize_for_new_map @0x837404D8 — reset the Blam decal globals (clearing the cluster decal grid to
 * "empty"), revalidate the decal pool, and rebuild the ws-engine decal bridge. */

#include <string.h>
#include "headers/decal_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);
extern void rasterizer_decals_initialize_for_new_map(void);

void decals_initialize_for_new_map(void)
{
    struct decal_globals *globals = decal_globals;  /* decal_globals is a bare struct tag (shadow global) */

    /* recovered: memset(globals, -1, 0x2800) -> named member (0x2800 == sizeof first_decal_indices) */
    memset(globals->first_decal_indices, -1, sizeof(globals->first_decal_indices)); /* clear the per-cluster decal index grid to -1 (no decal) */
    globals->locked_count = 0;
    globals->first_disconnected_decal_index = -1;
    globals->permanent_count = 0;
    data_make_valid(global_decal_data);
    rasterizer_decals_initialize_for_new_map(); /* attested void: r3-thread flattened */
}
