#pragma once

#include <stdint.h>
#include "data_array.h"
#include "lruv_cache.h"

/* DB anonymous struct: types_members _6F05A75B785DBFA3F6BCF7442DD673CA */
typedef struct pc_sound_cache_globals_t
{
    data_array *cache_sounds; // 0x00
    lruv_cache *cache;        // 0x04
    unsigned char bInitialized; // 0x08
} pc_sound_cache_globals_t;

extern pc_sound_cache_globals_t pc_sound_cache_globals;
extern unsigned int sound_cache_page_count; /* DB applied type: unsigned int */
extern int16_t sound_cache_size;
extern void sound_cache_delete_block_proc(int block_index);
extern uint8_t sound_cache_locked_block_proc(int block_index);
