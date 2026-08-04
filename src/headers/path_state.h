#pragma once

#include "path_input.h"
#include "path_destination.h"
#include "path_node.h"
#include "path_heap_element.h"
#include "real_point3d.h"
#include "structure_bsp.h"

/* forward-declared, not included: path_debug_storage embeds a path_state by value, so a full include here
 * would cycle. path_state only needs the pointer. */
typedef struct path_debug_storage path_debug_storage;

/* path_state — Blam pathfinding working set (65676 bytes in the database). `node_list`/`heap` offsets and
 * sizes confirmed via disasm_range in path_state_begin.c (node array: 0x84 + 52*i, exactly 1024 entries
 * ending at heap_count's own offset; heap array: heap_count's offset+2 + 4*i, DB path_heap_element[1025]). */
typedef struct path_state
{
    path_input            input;                 /* 0x00 */
    path_debug_storage    *debug;                 /* 0x48 */
    unsigned __int8       destination_valid;      /* 0x4C */
    unsigned char         _pad4D[3];              /* 0x4D */
    path_destination       destination;            /* 0x50 */
    const structure_bsp   *structure;              /* 0x64 */
    __int16               closest_node_index;     /* 0x68 */
    unsigned char         _pad6A[2];              /* 0x6A */
    float                  closest_distance;       /* 0x6C */
    float                  closest_cost_estimate;  /* 0x70 */
    real_point3d           closest_point;          /* 0x74 */
    __int16                node_count;             /* 0x80 (128) */
    unsigned char          _pad82[2];              /* 0x82 */
    path_node              node_list[1024];            /* 0x84 */
    __int16                heap_count;             /* 53380 */
    path_heap_element      heap[1025];             /* 53382 — DB: path_heap_element[1025] (4100 bytes) */
    __int16                hash_table[4096];       /* 57482 — node hash table, -1 = empty slot */
    unsigned char          _pad_c[65676 - (57482 + 8192)]; /* 65674 */
} path_state;                                      /* 65676 bytes */
