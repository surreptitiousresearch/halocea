#pragma once
/* structure_decals_globals_definition — runtime state for the per-cluster permanent-decal system.
 * reset_decals forces every visible cluster's decals to be respawned this frame. Layout from the database. */

typedef struct structure_decals_globals_definition
{
    unsigned __int8 reset_decals; /* 0x00 */
    unsigned __int8 pad[3];       /* 0x01 — pads struct to 4 bytes */
} structure_decals_globals_definition;

extern structure_decals_globals_definition *structure_decals_globals;
