/* hcex_machine_is_power_on @0x83681DD8 — true when a machine object (device_machine) is powered on. Resolves
 * the object datum via the object-header data-array (same idiom as hcex_machine_position), requires the
 * object's type to be 7 (machine), and reports powered when its device power scalar is strictly positive. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/machine_datum.h"
#include "../headers/object_type.h"

extern data_array *object_header_data;

extern "C" int hcex_machine_is_power_on(int id)
{
    machine_datum *object =
        (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    if ( object->object.type != object_type_machine )
        return 0;
    if ( object->device.power <= 0.0f )
        return 0;
    return 1;
}
