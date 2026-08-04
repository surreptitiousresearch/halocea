#pragma once
// DB-verified via types_members _C2C97468284848AA9AB75F912F876339 (anonymous PDB type)

/* Base addresses of the physical memory regions reserved at boot.
   IDA type: anonymous struct _C2C97468284848AA9AB75F912F876339 */
/* Shadow pattern: object name == type name → bare struct TAG only (no typedef). */
struct physical_memory_map_globals
{
    void *game_state_base_address; // 0x00
    void *tag_cache_base_address;  // 0x04
};

extern struct physical_memory_map_globals physical_memory_map_globals;
