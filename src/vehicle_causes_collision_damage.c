/* vehicle_causes_collision_damage @0x8375D7F8 — true if the vehicle's tag definition has the
 * "causes collision damage" flag (bit 7 of the flags dword at definition[188]). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/vehicle_definition_flags.h"
#include "headers/blam_data_globals.h"

#include <stdint.h>

/* attested uint8_t(uint16_t): caller byte-normalizes r3 (clrlwi 24 @837D5FE8); param first-use is
 * the datum-index mask clrlwi r10,r3,16 @8375D7FC */
uint8_t vehicle_causes_collision_damage(int vehicle_index)
{
    int definition_index = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum->definition_index;
    vehicle_definition *definition = TAG_GET(vehicle_definition, definition_index);
    return (definition->vehicle.flags >> _vehicle_causes_collision_damage_bit) & 1;   /* +752 */
}
