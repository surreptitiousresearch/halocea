#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* cache_file_runtime_globals — runtime state of the open cache (.map) file (anonymous DB type). Distinct from
 * cache_file_globals_s (the request/streaming side); this models the loaded header and tag/bsp header pointers. */

#include "cache_file_header.h"

typedef struct cache_file_tags_header cache_file_tags_header;
typedef struct cache_file_structure_bsp_header cache_file_structure_bsp_header;

typedef struct cache_file_runtime_globals
{
    unsigned __int8                  tags_loaded;          /* 0x000 */
    char                             _pad1[3];             /* 0x001 */
    cache_file_header                header;               /* 0x004 */
    cache_file_tags_header          *tags_header;          /* 0x804 */
    cache_file_structure_bsp_header *structure_bsp_header; /* 0x808 */
} cache_file_runtime_globals;

extern cache_file_runtime_globals cache_file_globals;
