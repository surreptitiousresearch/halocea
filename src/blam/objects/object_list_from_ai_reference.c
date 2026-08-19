/* object_list_from_ai_reference @0x8376F2C8 — build a HaloScript object list containing every unit referenced
 * by an AI index, including the swarm units chained off each actor (swarm chain via object data +0x1FC).
 * Returns the new object list, or -1 for an invalid AI index or list-allocation failure. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/blam_data_globals.h"

extern int object_list_new(void);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void object_list_add(int object_list_index, int object_index);

int object_list_from_ai_reference(int ai_index)
{
    if ( ai_index == -1 )
        return -1;

    int object_list = object_list_new();
    if ( object_list == -1 )
        return -1;

    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator); actor; actor = ai_index_actor_iterator_next(&iterator) )
    {
        if ( actor->meta.unit_index != -1 )
            object_list_add(object_list, actor->meta.unit_index);

        for ( int swarm_unit = actor->meta.swarm_unit_index; swarm_unit != -1; )
        {
            unit_datum *swarm_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, swarm_unit)->datum;
            object_list_add(object_list, swarm_unit);
            swarm_unit = swarm_data->unit.swarm_next_unit_index;
        }
    }
    return object_list;
}
