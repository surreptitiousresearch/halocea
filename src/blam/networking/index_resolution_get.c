/* index_resolution_get @0x83818C58 — look up the local object index mapped to `server_object_index` in an
 * index_resolution_table. The slot is chosen by |server_object_index| % number_of_slots, then its entry
 * chain is walked for an exact server-index match. Returns the entry's local index, or -1 if the table is
 * uninitialized, the input is -1, or no entry matches. */

#include "headers/index_resolution_table.h"

int index_resolution_get(const index_resolution_table *table, int server_object_index)
{
    if ( table->is_initialized != 1 || server_object_index == -1 )
        return -1;

    int hash = server_object_index;
    if ( server_object_index < 0 )
        hash = -server_object_index;

    index_entry *entry = table->slots[hash % table->number_of_slots].entries;
    if ( !entry )
        return -1;

    while ( entry->server_object_index != server_object_index )
    {
        entry = entry->next;
        if ( !entry )
            return -1;
    }
    return entry->local_object_index;
}
