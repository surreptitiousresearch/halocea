/* hcex_is_vehicle @0x83681C30 — true when the object with the given index is a vehicle (object type 1).
 * See hcex_is_weapon for the object-datum access pattern. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/object_datum.h"
#include "../headers/object_type.h"

extern "C" data_array *object_header_data;

extern "C" int hcex_is_vehicle(uint16_t id)
{
    return ((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum)->object.type == object_type_vehicle;
}
