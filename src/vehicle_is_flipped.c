/* vehicle_is_flipped @0x8375F5D8 */
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


BOOL vehicle_is_flipped(int vehicle_index)
{
    vehicle_datum *object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    return object->object.up.n[2] < 0.2f;
}
