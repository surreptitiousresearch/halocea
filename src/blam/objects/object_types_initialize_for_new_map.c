/* object_types_initialize_for_new_map @0x837016B8 — clear the processed-BSP flags and run each registered
 * object type's per-map init callback by walking the object_type_definition list. */

#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


/* ATTESTED void(void): sole caller (objects_initialize_for_new_map @0x836EEB80) ignores r3 and the
 * only r3 producer was the threaded callback result — decompiler r3-threading. The former
 * (int (*)(void)) cast on the header's void(*)(void) initialize_for_new_map fp existed only to
 * carry that phantom result; removed by restoring the header interface. */
void object_types_initialize_for_new_map(void)
{
    processed_bsp_flags = 0;
    for ( object_type_definition *type = first_object_type_definition; type; type = type->next )
    {
        if ( type->initialize_for_new_map )
            type->initialize_for_new_map();
    }
}
