/* object_deactivate @ 0x836EBC40 — clear one object datum's active bit (it left the combined PVS).
 * Datum layout as in object_activate: 12-byte header element, byte[2] holds the flags. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


void object_deactivate(int object_index)
{
    object_header_datum *object_header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);

    /* flags bit 0 == _object_header_active_bit */
    if ( (object_header->flags & (1u << _object_header_active_bit)) != 0 )
        object_header->flags &= ~(1u << _object_header_active_bit);
}
