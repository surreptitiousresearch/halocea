/* structure_detail_objects_initialize @ 0x837115C0 — allocate detail-object runtime
 * globals (42032 bytes) and seed the default +Z reference vector. */

#include "headers/detail_object_global_runtime_data.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled default_z_reference_vector@0; the DB-verified layout places it @0xA420
 * (after local_player_data[2]), and the 42032-byte alloc == sizeof the canonical struct. Reusing the
 * canonical header corrects the offset. */

extern void *game_state_malloc(const char *name, const char *type, int size);

void structure_detail_objects_initialize(void)
{
    struct detail_object_global_runtime_data *globals =
        game_state_malloc("structure detail objects", 0, 42032);
    globals->default_z_reference_vector.n[0] = 0.0f;
    detail_object_global_runtime_data = globals;
    globals->default_z_reference_vector.n[1] = 0.0f;
    globals->default_z_reference_vector.n[2] = 1.0f;
    globals->default_z_reference_vector.n[3] = 0.0f;
}
