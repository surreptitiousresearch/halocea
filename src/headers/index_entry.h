#pragma once
/* index_entry — one server-index -> local-index mapping node in an index_resolution_table hash chain. */

typedef struct index_entry
{
    int                 server_object_index;   /* 0x0 */
    int                 local_object_index;    /* 0x4 */
    struct index_entry *next;                  /* 0x8 — next entry in the same slot chain */
} index_entry;
