/* widgets_need_lighting 0x83756410 — walk an object's widget sibling chain (antennas, fluids, light volumes,
 * etc.) and report whether any widget's type requires the object's render lighting to be computed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/widget_datum.h"
#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"


uint8_t widgets_need_lighting(int widget_index)
{
    if ( widget_index == -1 )
        return 0;

    while ( 1 )
    {
        widget_datum *widget = DATA_ARRAY_ELEMENT(widget_data, widget_datum, widget_index);
        if ( widget_type_definitions[widget->type].needs_lighting )
            return 1;
        widget_index = widget->next_widget_index;
        if ( widget_index == -1 )
            return 0;
    }
}
