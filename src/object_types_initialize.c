/* object_types_initialize @ 0x83701580 — build the object-type definition linked list
 * (each type followed by its part definitions) and run each type's initializer.
 *
 * partial: object_type_definition fields (next, part_definitions[16], initialize) are
 * modeled at assumed offsets; the part-definition `next == NULL` test selects parts not
 * already linked. */

#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled a 3-field {next@0, part_definitions@4, initialize@68} object type; the
 * DB-verified vtable (object_type_definition.h) has part_definitions@0x80 and next@0xC0. Reusing the
 * canonical header corrects the offsets (field names match). */


/* ATTESTED void(void): no arg register is read before written (disasm 0x83701580) and the sole
 * caller (objects_initialize @0x836EEA80) ignores r3 — the old `chained` param/return was
 * decompiler r3-threading of a dead register. */
void object_types_initialize(void)
{
    object_type_definition **tail = &first_object_type_definition;
    object_type_definition *type;
    short type_index;

    for ( type_index = 0; type_index < 12; ++type_index )
    {
        int part_index;
        type = object_type_definitions[type_index];
        *tail = type;
        tail = &type->next;
        for ( part_index = 0; part_index < 16; ++part_index )
        {
            object_type_definition *part = type->part_definitions[part_index];
            if ( !part )
                break;
            if ( !part->next )
            {
                *tail = part;
                tail = &part->next;
            }
        }
    }
    *tail = 0;

    for ( type = first_object_type_definition; type; type = type->next )
    {
        if ( type->initialize )
            /* attested void(void): bctrl @0x83701638 sets up no argument registers; the header's
             * former 4-param fp type was phantom decompiler threading (fixed centrally) */
            type->initialize();
    }
}
