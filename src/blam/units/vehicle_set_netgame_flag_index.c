/* vehicle_set_netgame_flag_index @0x83761050 — writes a vehicle datum's vehicle_netgame_flag_index
 * (union at _vehicle_datum +0xE4 / vehicle_datum +1456). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


void vehicle_set_netgame_flag_index(int vehicle_index, int16_t netgame_flag_index)
{
    vehicle_datum *object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    object->vehicle.___u27.vehicle_netgame_flag_index = netgame_flag_index;
}
