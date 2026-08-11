/* add_pool @0x83818A10 — grows an index_resolution_table's free list by allocating one more pool of 50
 * index_entry records, linking the pool into the table's pool list, and pushing every record (initialized
 * empty) onto the free list. The original loop is unrolled 5x (10 iterations of 5 entries); reproduced as
 * a single 50-iteration loop. */

#include "headers/index_resolution_table.h"
#include "headers/index_entry_pool.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

#define INDEX_RESOLUTION_SOURCE "D:\\Projects\\code\\HCEX\\sources\\objects\\index_resolution.c"

void add_pool(index_resolution_table *table)
{
    index_entry_pool *pool = dlMalloc(8u, INDEX_RESOLUTION_SOURCE, 0x13Bu);
    pool->memory = dlMalloc(0x258u, INDEX_RESOLUTION_SOURCE, 0x13Cu);  /* 50 * sizeof(index_entry) */
    pool->next = table->entry_pool_list;
    table->entry_pool_list = pool;

    index_entry *entries = (index_entry *)pool->memory;
    for (int i = 0; i < 50; ++i)
    {
        index_entry *entry = &entries[i];
        entry->server_object_index = -1;
        entry->local_object_index = -1;
        entry->next = table->entry_free_list;
        table->entry_free_list = entry;
    }
}
