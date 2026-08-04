#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* cache_file_globals (tag-system instance) @ cache_file_globals — tracks the loaded tag/cache
 * file state. Distinct from the streaming cache_file_globals_s; this is the tag-resolution side. */

#include "cache_file_header.h"
#include "cache_file_tags_header.h"

typedef struct cache_file_structure_bsp_header cache_file_structure_bsp_header;

typedef struct cache_file_tags_globals
{
    unsigned char                    tags_loaded;          /* 0x000 */
    cache_file_header                header;               /* 0x004 (2048 bytes) */
    cache_file_tags_header          *tags_header;          /* 0x804 */
    cache_file_structure_bsp_header *structure_bsp_header; /* 0x808 */
} cache_file_tags_globals;

extern cache_file_tags_globals cache_file_globals;
