/* device_get_position @0x837B4E38 — return a device object's current position/openness value
 * (device.position), or 0 for an invalid device. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/blam_data_globals.h"


float device_get_position(int device_index)
{
    if ( device_index == -1 )
        return 0.0f;
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    return device->device.position;
}
