/* device_touched @0x837B5164 — dispatch a unit's "use" interaction on a device object to the right handler
 * based on the device's object type (machine = 7, control = 8). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void machine_bumped(int machine_index, int unit_index);
extern void control_touched(int control_index, int unit_index);

void device_touched(int device_index, int unit_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    int object_type = (unsigned __int16)object->object.type;
    if ( object_type == object_type_machine )
        machine_bumped(device_index, unit_index);
    else if ( object_type == object_type_control )
        control_touched(device_index, unit_index);
}
