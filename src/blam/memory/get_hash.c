/* get_hash @0x838188E8 */
#include "headers/index_resolution_table.h"

int get_hash(index_resolution_table *table, int object_index)
{
    if (object_index < 0)
        object_index = -object_index;
    return object_index % table->number_of_slots;
}
