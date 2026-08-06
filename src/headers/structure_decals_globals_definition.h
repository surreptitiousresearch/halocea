#pragma once
#include <stdint.h>
/* structure_decals_globals_definition — runtime state for the per-cluster permanent-decal system.
 * reset_decals forces every visible cluster's decals to be respawned this frame. Layout from the database. */

typedef struct structure_decals_globals_definition
{
    uint8_t reset_decals; /* 0x00 */
    uint8_t pad[3];       /* 0x01 — pads struct to 4 bytes */
} structure_decals_globals_definition;

#ifdef __cplusplus
extern "C" {
#endif

extern structure_decals_globals_definition *structure_decals_globals;

#ifdef __cplusplus
}
#endif
