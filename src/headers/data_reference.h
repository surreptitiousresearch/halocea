#pragma once
/* data_reference — a generic 12-byte data_array reference-datum used by cluster
 * partitions and reference lists. No named type exists in the DB; the field layout
 * is reconstructed from the observed accesses (salt datum-header at +0, a payload
 * index at +4, a next-reference link at +8). The `value` field carries either a
 * cluster index or a referenced-datum index depending on the owning array. */

typedef struct data_reference
{
    __int16 identifier;          /* 0x00: data_array datum salt */
    unsigned __int16 pad;        /* 0x02 */
    int datum_index;             /* 0x04: cluster/datum index (DB name) */
    int next_reference_index;    /* 0x08: -1 terminates the chain */
} data_reference;
