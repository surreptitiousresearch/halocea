/* object_get_function_value @0x836EC4D0 — read one of an object's exported animation/function outputs.
 * function_index -1 yields a constant 1.0 (always active). Otherwise the value is read from the object
 * datum's outgoing_function_values table and the return reports whether that function is currently
 * active (functions_active_flags bit mask). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


/* caller clrlwi r3,24 after call (effect_update @836E3850) => 8-bit boolean return */
uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = object_header->datum;
    if ( function_index == -1 )
    {
        *value_reference = 1.0f;
        return 1;
    }
    *value_reference = object->object.outgoing_function_values[function_index];
    return (object->object.functions_active_flags & (1 << function_index)) != 0;
}
