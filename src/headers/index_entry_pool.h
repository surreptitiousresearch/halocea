#pragma once
/* index_entry_pool — one allocated block of index_entry storage, chained into the table's pool list so it
 * can be freed when the table is disposed (DB layout). */

typedef struct index_entry_pool
{
    void                     *memory;   /* 0x0 — block holding 50 index_entry records */
    struct index_entry_pool  *next;     /* 0x4 — next pool in the table's list */
} index_entry_pool;
