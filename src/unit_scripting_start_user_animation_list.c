#include <stdint.h>
#include "headers/object_type.h"
/* unit_scripting_start_user_animation_list @0x83800B20 — start a user animation on every unit in an object
 * list; returns true only if it was started successfully on every unit (logical AND across the list). */

extern int object_list_get_first(int object_list_index, int *iterator);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

uint8_t unit_scripting_start_user_animation_list(int object_list_index, int animation_graph_index, const char *animation_name, uint8_t interpolate)
{
    unsigned char all_started = 1;
    int iterator[16];
    for ( int object_index = object_list_get_first(object_list_index, iterator);
          object_index != -1;
          object_index = object_list_get_next(object_list_index, iterator) )
    {
        if ( object_try_and_get_and_verify_type(object_index, object_mask_unit) )
        {
            all_started = all_started
                       && unit_start_user_animation(object_index, animation_graph_index, animation_name, interpolate);
        }
    }
    return all_started;
}
