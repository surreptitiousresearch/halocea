#pragma once
/* _field_type_translated_index_parameters — parameters blob for the "translated index" field type: a
 * client-local index gets registered against a network-shared translated index, allocated from a small
 * pool and resolved through an index_resolution_table. DB layout, 40 bytes header + trailing allocations. */

#include "index_resolution_table.h"

typedef struct _field_type_translated_index_parameters
{
    const int               maximum_active_at_once;             /* 0x00 */
    const int               initial_translation_table_size;     /* 0x04 */
    int                      number_of_bits;                     /* 0x08 */
    index_resolution_table  translation_table;                   /* 0x0C */
    int                      translated_index_allocation_cursor; /* 0x24 */
    int                     *translated_index_allocations;       /* 0x28 */
} _field_type_translated_index_parameters;
