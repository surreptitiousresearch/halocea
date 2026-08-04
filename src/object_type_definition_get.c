/* object_type_definition_get @0x83700B30 — return the object_type_definition for an object type enum
 * by indexing the global table. */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


object_type_definition *object_type_definition_get(int16_t object_type)
{
    return object_type_definitions[object_type];
}
