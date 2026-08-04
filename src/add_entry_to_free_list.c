#include "headers/index_resolution_table.h"
#include "headers/index_entry.h"

void add_entry_to_free_list(index_resolution_table *table, index_entry *entry)
{
    entry->server_object_index = -1;
    entry->local_object_index = -1;
    entry->next = table->entry_free_list;
    table->entry_free_list = entry;
}
