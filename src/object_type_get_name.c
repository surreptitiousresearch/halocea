/* object_type_get_name @0x83700B68 — return the human-readable name string for an object type, from the
 * object type definition table. */

#include <stdint.h>
#include "headers/object_type_definition.h"


const char * object_type_get_name(int16_t object_type)
{
    return object_type_definitions[object_type]->name;
}
