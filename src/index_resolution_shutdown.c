/* index_resolution_shutdown @0x83818848 — tear down an index_resolution_table: clear every slot's bucket,
 * free every pool block in the entry-pool chain, then free the slot array itself. No-op if not
 * initialized (mirrors index_resolution_initialize.c). */

#include "headers/index_resolution_table.h"
#include "headers/index_entry_pool.h"

extern void dlFree(void *ptr);

void index_resolution_shutdown(index_resolution_table *table)
{
    if ( table->is_initialized != 1 )
        return;

    for ( int i = 0; i < table->number_of_slots; ++i )
    {
        table->slots[i].entries = 0;
        table->slots[i].number_of_entries = 0;
    }

    index_entry_pool *pool = table->entry_pool_list;
    table->entry_free_list = 0;

    while ( pool )
    {
        index_entry_pool *next = pool->next;
        dlFree(pool->memory);
        pool->memory = 0;
        dlFree(pool);
        pool = next;
    }

    table->entry_pool_list = 0;
    dlFree(table->slots);
    table->slots = 0;
    table->number_of_slots = 0;
    table->total_number_of_entries = 0;
    table->is_initialized = 0;
}
