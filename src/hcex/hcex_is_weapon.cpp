/* hcex_is_weapon @0x83681BF8 — true when the object with the given index is a weapon (object type 2).
 * The object header datum is a 12-byte (3-dword) record; dword[2] is the object data pointer, whose
 * type field lives at offset 180. */

#include <stdint.h>
#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/object_datum.h"
#include "../headers/object_type.h"

extern data_array *object_header_data;

extern "C" int hcex_is_weapon(uint16_t id)
{
    return ((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum)->object.type == object_type_weapon;
}
