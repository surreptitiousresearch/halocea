/* vehicle_is_at_rest_for_respawn_purposes @0x8375CF38 — bit 5 of the vehicle object's +16 flags word
 * (same raw-offset convention as vehicle_update.c: "+16 object flags", accessed opaquely project-wide). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


/* DEVIATION (2026-08-12, #134): vehicle_index is `int`. This function has 0 xrefs of any kind, so it
 * has no caller evidence of its own — but it subscripts `object_header_data` (`clrlwi r11, r3, 16`
 * @0x8375CF3C -> `mulli r10, r11, 0xC`), which makes it a member of the OBJECT handle family, not a
 * family of one. That family's boundary was resolved this session:
 * structure_visibility_find_objects @0x837C4DA8 stores object_header_data indices WHOLE into
 * `int *result_indices` (`stwx r31, r11, r25` @0x837C4E98) and passes them whole to
 * get_bounding_sphere(int, ...) and to both mark callbacks. funcs.prototype agrees independently:
 * `unsigned __int8 vehicle_is_at_rest_for_respawn_purposes(int vehicle_index)`.
 * INHERITED, not individually verified — the `clrlwi ...,16` above is DATA_ARRAY_ELEMENT's own
 * truncation (data_array.h) and says nothing about the ABI either way. */
uint8_t vehicle_is_at_rest_for_respawn_purposes(int vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    return (vehicle->object.flags >> _object_at_rest_bit) & 1;
}
