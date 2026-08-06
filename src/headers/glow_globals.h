#pragma once
// DB-verified via types_members _9733C5510C665881AB480F38E7DF5C45 (anonymous PDB type)
/* glow_globals — glow effect system runtime state (glow.c). Layout from disasm (glow_dispose_from_old_map/
 * glow_initialize_for_new_map): glow_data at +0x0, glow_particle_data at +0x4. */

#include "data_array.h"

typedef struct glow_globals_type
{
    data_array *glow_data;          /* 0x00 */
    data_array *glow_particle_data; /* 0x04 */
} glow_globals_type;

#ifdef __cplusplus
extern "C" {
#endif

extern glow_globals_type glow_globals;

#ifdef __cplusplus
}
#endif
