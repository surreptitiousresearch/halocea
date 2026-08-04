/* hcex_machine_is_locked @0x83681D30 — is the given machine object locked? Resolves the object datum via the
 * object-header data-array, requires it to be a machine, then scans the device_machine tag definition's
 * device.function_modes[4] table for the "locked" function mode. If found, the machine is reported locked
 * when the matching device scalar in the object datum (incoming_function_values[slot]) is strictly positive.
 * Mirrors hcex_machine_is_power_on.
 * Typed via DB machine_definition: .device (_device_definition) @ +380, .function_modes __int16[4] @ +28
 * (380+28 = 408 = the word index 204 the decompiler showed). */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/machine_datum.h"
#include "../headers/machine_definition.h"
#include "../headers/global_tag_instances.h"
#include "../headers/object_type.h"
#include "../headers/device_function_mode.h"

extern data_array *object_header_data;

extern "C" int hcex_machine_is_locked(int id)
{
    machine_datum *object =
        (machine_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;
    if ( object->object.type != object_type_machine )
        return 0;

    const machine_definition *definition =
        *(const machine_definition **)TAG_INSTANCE(object->definition_index);

    int slot = 0;
    while ( definition->device.function_modes[slot] != _device_function_locked )
    {
        slot = (__int16)(slot + 1);
        if ( slot >= 4 )   /* 4 == countof(function_modes) */
            return 0;
    }

    if ( object->object.incoming_function_values[slot] <= 0.0f )
        return 0;
    return 1;
}
