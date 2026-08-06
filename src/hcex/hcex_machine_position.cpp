/* hcex_machine_position @0x83681CE8 — return a machine object's device position scalar, or 0 when the
 * object's type is not 7 (the machine type). Resolves the object datum via the object-header data-array. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/machine_datum.h"
#include "../headers/object_type.h"

extern "C" data_array *object_header_data;

extern "C" float hcex_machine_position(int id)
{
    machine_datum *object =
        (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    if ( object->object.type == object_type_machine )
        return object->device.position;
    return 0.0f;
}
