/* widgets_dispose_from_old_map @0x837560C8 — per-map widget teardown: call each of the 5 widget type
 * definitions' dispose-from-old-map hooks, then invalidate the widget datum array. */

#include "headers/widget_type_definition.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_make_invalid(data_array *data);

void widgets_dispose_from_old_map(void)
{
    for ( int i = 0; i < 5; i = (short)(i + 1) )
    {
        if ( widget_type_definitions[i].dispose_from_old_map )
            widget_type_definitions[i].dispose_from_old_map();
    }
    data_make_invalid(widget_data);
}
