#pragma once
// DB-verified via types_members _152E52BAA6B276FAC0EAB51D36176939 (anonymous PDB type)

#include "data_array.h"
#include "lruv_cache.h"

typedef struct pc_texture_cache_globals_struct
{
    data_array *textures; // 0x00
    lruv_cache *cache;    // 0x04
} pc_texture_cache_globals_struct;

#ifdef __cplusplus
extern "C" {
#endif

extern pc_texture_cache_globals_struct pc_texture_cache_globals;

#ifdef __cplusplus
}
#endif
