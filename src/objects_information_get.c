/* objects_information_get @0x836EC8B8 — tally the live object table: total objects, active objects, and the
 * fraction of the object memory pool currently in use. A nonzero object_header_datum.identifier marks a
 * live slot; object_header_datum.flags _object_header_active_bit marks active.
 * DEVIATION: the soft-float __SPAIR64__ idiom is the int->float conversion of the contiguous free size;
 * used_memory = 1 - free_size/2097152 (the pool's 2 MB capacity). */

#include "headers/data_array.h"
#include "headers/objects_information.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/memory_pool.h"
#include "headers/blam_data_globals.h"

extern int memory_pool_get_contiguous_free_size(memory_pool *pool);

void objects_information_get(objects_information *information)
{
    information->object_count = 0;
    information->active_object_count = 0;
    information->used_memory = 0.0f;

    object_header_datum *entry = (object_header_datum *)object_header_data->data;
    if ( object_header_data->count > 0 )
    {
        int i = 0;
        do
        {
            if ( entry->identifier )   /* live slot */
            {
                ++information->object_count;
                if ( (entry->flags & (1u << _object_header_active_bit)) != 0 )
                    ++information->active_object_count;
            }
            ++entry;
            i = (__int16)(i + 1);
        }
        while ( i < object_header_data->count );
    }

    information->used_memory = -((float)memory_pool_get_contiguous_free_size(object_memory_pool) * 0.00000047683716f - 1.0f);
}
