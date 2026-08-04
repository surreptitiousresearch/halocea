/* vehicle_is_at_rest_for_respawn_purposes @0x8375CF38 — bit 5 of the vehicle object's +16 flags word
 * (same raw-offset convention as vehicle_update.c: "+16 object flags", accessed opaquely project-wide). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t vehicle_is_at_rest_for_respawn_purposes(uint16_t vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    return (vehicle->object.flags >> _object_at_rest_bit) & 1;
}
