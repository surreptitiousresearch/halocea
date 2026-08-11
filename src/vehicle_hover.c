/* vehicle_hover @0x8375D9A0 — enable or disable a vehicle's hover mode (object flag bit 0x2 at object
 * word +1228). When enabling, capture the current origin as the hover anchor point (object +1276). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/real_point3d.h"
#include "headers/vehicle_flags.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

void vehicle_hover(int vehicle_index, uint8_t hover_flag)
{
    if ( vehicle_index != -1 )
    {
        vehicle_datum *vehicle = (vehicle_datum *)
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
        short flags;
        if ( hover_flag )
        {
            object_get_origin(vehicle_index, &vehicle->vehicle.hover_position);
            flags = vehicle->vehicle.flags | (1u << _vehicle_hovering_bit);
        }
        else
        {
            flags = vehicle->vehicle.flags & ~(1u << _vehicle_hovering_bit);
        }
        vehicle->vehicle.flags = flags;
    }
}
