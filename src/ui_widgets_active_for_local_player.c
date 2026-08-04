/* ui_widgets_active_for_local_player @0x837308B0 — the decompiler rendered this as a raw pointer walk over
 * widget_globals starting at &active_widgets[0] and stepping by 4 bytes until reaching widget_stack; that is
 * exactly an iteration over the 2-element active_widgets array (widget_stack immediately follows it in
 * widget_globals.h), reproduced here as the equivalent array loop. */

#include <stdint.h>
#include "headers/widget_globals.h"

int ui_widgets_active_for_local_player(int16_t local_player_index)
{
    if ( !widget_globals.initialized )
        return 0;

    for ( int i = 0; i < 2; i++ )
    {
        if ( widget_globals.active_widgets[i] && widget_globals.active_widgets[i]->local_player_index == local_player_index )
            return 1;
    }
    return 0;
}
