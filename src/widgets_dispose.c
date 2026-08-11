/* widgets_dispose @0x83756148 — one-time shutdown: call each of the 5 widget type definitions' dispose
 * hooks.
 *
 * The dispose hooks are void(void) (widget_type_definition.dispose; disasm-confirmed: the sole caller
 * ignores r3), so this proc is void too — the decompiler's threaded "return" was uninitialized r3
 * residue, not a real status. */

#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"


void widgets_dispose(void)
{
    for ( int i = 0; i < 5; i = (short)(i + 1) )
    {
        if ( widget_type_definitions[i].dispose )
            widget_type_definitions[i].dispose();
    }
}
