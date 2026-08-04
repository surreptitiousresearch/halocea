/* object_type_get_update_message_type @0x83700B88 — return the network update-message type registered for
 * a given object type code, by indexing the object_type_definitions table. Used to decide whether an object
 * participates in networked state replication (a result of -1 means it does not). */

#include <stdint.h>
#include "headers/object_type_definition.h"

message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type)
{
    return object_type_definitions[object_type]->update_message_type;
}
