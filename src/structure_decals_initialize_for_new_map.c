/* structure_decals_initialize_for_new_map @0x837E2D30 — clear the per-map decal-reset request. */

#include "headers/structure_decals_globals_definition.h"

void structure_decals_initialize_for_new_map(void)
{
    structure_decals_globals->reset_decals = 0;
}
