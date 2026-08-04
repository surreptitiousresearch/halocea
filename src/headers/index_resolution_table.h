#pragma once
/* index_resolution_table — maps externally-visible "local" indices to translated network/global indices via
 * a slot table backed by a pooled free list (used by the field-properties index-translation layer). */

#include "index_slot.h"
#include "index_entry.h"

struct index_entry_pool;

typedef struct index_resolution_table
{
    unsigned __int8          is_initialized;            /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    int                      number_of_slots;           /* 0x04 */
    index_slot              *slots;                     /* 0x08 */
    int                      total_number_of_entries;   /* 0x0C */
    index_entry             *entry_free_list;           /* 0x10 */
    struct index_entry_pool *entry_pool_list;           /* 0x14 */
} index_resolution_table;
