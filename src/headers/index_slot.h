#pragma once
/* index_slot — one bucket of an index_resolution_table: a count plus the head of its index_entry chain. */

#include "index_entry.h"

typedef struct index_slot
{
    int          number_of_entries;   /* 0x0 */
    index_entry *entries;             /* 0x4 — head of the hash-chain for this bucket */
} index_slot;
