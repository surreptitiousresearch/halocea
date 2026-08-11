/* hs_trigger_volume_test_objects @0x837F7810 — script primitive: test every object in an object list
 * against a trigger volume. With `and` set, returns true only if all objects are inside (short-circuit
 * false on the first miss); with `and` clear (OR), returns true if any object is inside (short-circuit
 * true on the first hit). An empty list returns `and`. */

#include <stdint.h>

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);

int hs_trigger_volume_test_objects(int16_t trigger_volume_index, int object_list_index, int and)
{
    int reference_iterator[12];
    int object_index = object_list_get_first(object_list_index, reference_iterator);
    if ( object_index == -1 )
        return and;

    while ( 1 )
    {
        if ( scenario_trigger_volume_test_object(trigger_volume_index, object_index) )
        {
            if ( !(unsigned char)and )
                return 1;
        }
        else
        {
            if ( (unsigned char)and )
                return 0;
        }
        object_index = object_list_get_next(object_list_index, reference_iterator);
        if ( object_index == -1 )
            return and;
    }
}
