/* get_slot @0x838189E0 — hash bucket for an object index (abs value mod slot count). */

#include "headers/index_resolution_table.h"

index_slot * get_slot(index_resolution_table *table, int object_index)
{
    if (object_index < 0)
        object_index = -object_index;
    return &table->slots[object_index % table->number_of_slots];
}
