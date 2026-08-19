/* index_resolution_set @0x83818B18 — inserts, updates, or removes a server->local object-index mapping in
 * a hash table keyed by server object index. Passing local_object_index == -1 removes the mapping (and
 * recycles its entry to the free list); otherwise an existing entry is updated in place, or a new entry
 * is taken from the free list (growing the pool if empty) and pushed onto the slot's chain. No-op if the
 * table is not initialized. */

#include "headers/index_resolution_table.h"

extern void add_pool(index_resolution_table *table);

void index_resolution_set(index_resolution_table *table, int server_object_index, int local_object_index)
{
    if (table->is_initialized != 1)
        return;

    int hash_key = server_object_index < 0 ? -server_object_index : server_object_index;
    char need_new_entry = 1;
    index_entry *previous = nullptr;
    index_slot *slot = &table->slots[hash_key % table->number_of_slots];

    index_entry *entry = slot->entries;
    if (entry)
    {
        while (entry->server_object_index != server_object_index)
        {
            previous = entry;
            entry = entry->next;
            if (!entry)
                goto maybe_insert;
        }

        if (local_object_index == -1)
        {
            index_entry *next = entry->next;
            if (previous)
                previous->next = next;
            else
                slot->entries = next;
            entry->server_object_index = -1;
            entry->local_object_index = -1;
            entry->next = table->entry_free_list;
            table->entry_free_list = entry;
            --slot->number_of_entries;
            --table->total_number_of_entries;
        }
        else
        {
            entry->local_object_index = local_object_index;
        }
        need_new_entry = 0;
    }

maybe_insert:
    if (need_new_entry == 1)
    {
        if (!table->entry_free_list)
            add_pool(table);
        index_entry *fresh = table->entry_free_list;
        table->entry_free_list = fresh->next;
        fresh->server_object_index = server_object_index;
        fresh->local_object_index = local_object_index;
        fresh->next = slot->entries;
        slot->number_of_entries = slot->number_of_entries + 1;
        slot->entries = fresh;
        ++table->total_number_of_entries;
    }
}
