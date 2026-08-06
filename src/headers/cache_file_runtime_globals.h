#pragma once
/* DB-verified: anonymous PDB struct $F4497E3014C013DA64EA5AA79E59B767 (types.size = 2060), all
 * four members and offsets taken from types_members. (This header previously carried a
 * "Reconstruction (no DB/PDB type)" note; the type IS in the DB, keyed by the address of
 * cache_file_globals @ 0x8445C050 via applied_types, which is why a name lookup missed it.) */
/* cache_file_runtime_globals — runtime state of the open cache (.map) file. Distinct from
 * cache_file_globals_s (the request/streaming side); this models the loaded header and tag/bsp
 * header pointers. `cache_file_tags_globals` is a typedef alias of this same type. */

#include <stdint.h>
#include "cache_file_header.h"

typedef struct cache_file_tags_header cache_file_tags_header;
typedef struct cache_file_structure_bsp_header cache_file_structure_bsp_header;

typedef struct cache_file_runtime_globals
{
    uint8_t                          tags_loaded;          /* 0x000 */
    char                             _pad1[3];             /* 0x001 */
    cache_file_header                header;               /* 0x004 */
    cache_file_tags_header          *tags_header;          /* 0x804 */
    cache_file_structure_bsp_header *structure_bsp_header; /* 0x808 */
} cache_file_runtime_globals;                              /* 0x80C = 2060 */

#ifdef __cplusplus
extern "C" {
#endif

extern cache_file_runtime_globals cache_file_globals;

#ifdef __cplusplus
}
#endif
