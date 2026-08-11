/* control_touched @0x837E9C58 — a unit touched a device control: toggle it only when its definition's
 * control.trigger (definition +0x292) is 0 (touch-triggered). unit_index is unused here. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/control_definition.h"
#include "headers/blam_data_globals.h"

extern void control_toggle(int control_index);

void control_touched(int control_index, int unit_index)
{
    object_datum *control = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, control_index)->datum;
    control_definition *definition = TAG_GET(control_definition, control->definition_index);
    if ( !definition->control.trigger )
        control_toggle(control_index);
}
