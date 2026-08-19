/* index_resolution_initialize @0x83818788 — one-time setup of an index_resolution_table: allocate its slot
 * array, zero every slot's bucket, and reset the entry pool bookkeeping. No-op if already initialized. */

#include "headers/index_resolution_table.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

void index_resolution_initialize(index_resolution_table *table, int number_of_slots)
{
    if ( table->is_initialized )
        return;

    table->number_of_slots = number_of_slots;
    table->slots = dlMalloc(8 * number_of_slots,
        "D:\\Projects\\code\\HCEX\\sources\\objects\\index_resolution.c", 0x19u);

    for ( int i = 0; i < table->number_of_slots; i++ )
    {
        table->slots[i].number_of_entries = 0;
        table->slots[i].entries = 0;
    }

    table->total_number_of_entries = 0;
    table->entry_free_list = 0;
    table->entry_pool_list = 0;
    table->is_initialized = 1;
}
