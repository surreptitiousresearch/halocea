/* object_types_dispose_from_old_map @0x83701718 — per-map teardown: call each registered object type's
 * dispose-from-old-map hook, walking the object-type definition list.
 *
 * The dispose-from-old-map hooks are void(void) (object_type_definition.dispose_from_old_map;
 * disasm-confirmed: the sole caller ignores r3), so this proc is void too — the decompiler's threaded
 * "return" was uninitialized r3 residue, not a real status. */

#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_types_dispose_from_old_map(void)
{
    for ( object_type_definition *type = first_object_type_definition; type; type = type->next )
    {
        if ( type->dispose_from_old_map )
            type->dispose_from_old_map();
    }
}
