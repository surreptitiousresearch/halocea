/* objects_paparazzi @0x836F1098 — sweep all objects and delete every "paparazzi" object
 * (_object_movie_star_bit) whose object.datum_role is 0 or 3, unregistering it from the network
 * first when datum_role is 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_iterator.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern void object_delete_to_network(int object_index);
extern void object_delete_initial_recursive(int object_index, uint8_t delete_siblings);

void objects_paparazzi(void)
{
    object_iterator iterator;
    iterator.flags = 0;
    iterator.signature = -2038004090;
    iterator.type_flags = -1;
    iterator.absolute_index = 0;
    iterator.index = -1;

    for ( object_datum *object = object_iterator_next(&iterator); object;
          object = object_iterator_next(&iterator) )
    {
        if ( (object->object.flags & (1u << _object_movie_star_bit)) == 0 )
            continue;

        int object_index = iterator.index;
        int state = object->object.datum_role;
        if ( !state )
        {
            object_delete_to_network(iterator.index);
            object_delete_initial_recursive(object_index, 0);
        }
        else if ( state == 3 )
        {
            object_delete_initial_recursive(object_index, 0);
        }
    }
}
