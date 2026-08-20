/* index_resolution_remove_local_index @0x83818928 — search every slot's hash chain for an entry mapping to
 * `local_object_index`, and if found, unlink it, clear both its indices, and return it to the table's free
 * list (also decrementing the slot and table entry counts). No-op if the table isn't initialized or no
 * entry matches. */

#include "headers/index_resolution_table.h"

void index_resolution_remove_local_index(index_resolution_table *table, int local_object_index)
{
    if ( table->is_initialized != 1 )
        return;

    for ( int slot_index = 0; slot_index < table->number_of_slots; ++slot_index )
    {
        index_slot *slot = &table->slots[slot_index];
        index_entry *entry = slot->entries;
        if ( !entry )
            continue;

        while ( entry->local_object_index != local_object_index )
        {
            entry = entry->next;
            if ( !entry )
                goto next_slot;
        }

        slot->entries = entry->next;
        entry->server_object_index = -1;
        entry->local_object_index = -1;
        entry->next = table->entry_free_list;
        table->entry_free_list = entry;
        --slot->number_of_entries;
        --table->total_number_of_entries;
        return;

next_slot:;
    }
}
