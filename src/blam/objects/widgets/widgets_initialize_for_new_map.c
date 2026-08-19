/* widgets_initialize_for_new_map @0x83756048 — revalidate the widget datum array and run each widget type's
 * per-map init callback (5 widget types: e.g. contrail, light-volume). */

#include <stdint.h>
#include "headers/widget_type_definition.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern void data_make_valid(data_array *data);

void widgets_initialize_for_new_map(void)
{
    data_make_valid(widget_data);
    for ( int16_t i = 0; i < 5; i = (int16_t)(i + 1) )
    {
        void (*initialize_for_new_map)(void) = widget_type_definitions[i].initialize_for_new_map;
        if ( initialize_for_new_map )
            initialize_for_new_map();
    }
}
