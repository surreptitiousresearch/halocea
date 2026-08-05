/* object_type_new @0x83701820 — run each part definition's datum_new callback (super-type then
 * sub-types) for a new object. If any callback fails (returns 0) the chain stops and 0 is returned;
 * 1 if all parts initialized (or there are none).
 *
 * Object data type word at +180 selects the object_type_definition. Deviation: the decompiler walks
 * part_definitions via raw pointer arithmetic; reconstructed as null-terminated array iteration. */

#include <stdint.h>
#include "headers/object_type_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


/* ATTESTED uint8_t return: the sole caller (object_new_with_datum_role_control @0x836F1A9C)
 * byte-normalizes r3 with clrlwi r3,24 after the call — declared return is 8-bit (Blam boolean). */
uint8_t object_type_new(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_type_definition *definition = object_type_definitions[object_header->datum->object.type];
    int i;

    for ( i = 0; definition->part_definitions[i]; ++i )
    {
        uint8_t (*datum_new)(int) = definition->part_definitions[i]->datum_new;
        if ( datum_new && !datum_new(object_index) )
            return 0;
    }
    return 1;
}
