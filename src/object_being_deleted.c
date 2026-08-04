/* object_being_deleted @0x836EBDB8 — test the object's "being deleted" flag
 * (object_header_datum.flags @+2, bit 3). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


/* caller clrlwi r3,24 after call => declared 8-bit boolean return */
uint8_t object_being_deleted(int object_index)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    return (header->flags >> _object_header_being_deleted_bit) & 1;
}
