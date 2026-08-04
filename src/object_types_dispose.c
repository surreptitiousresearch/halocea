/* object_types_dispose @0x8370167C — one-time shutdown: call each registered object type's dispose
 * hook, walking the object-type definition list.
 *
 * The dispose hooks are void(void) (object_type_definition.dispose; disasm-confirmed: registered
 * targets are void and the sole caller ignores r3), so this proc is void too — the decompiler's
 * threaded "return" was uninitialized r3 residue, not a real status. */

#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


void object_types_dispose(void)
{
    for ( object_type_definition *type = first_object_type_definition; type; type = type->next )
    {
        if ( type->dispose )
            type->dispose();
    }
}
