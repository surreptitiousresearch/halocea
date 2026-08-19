/* control_destroyed @0x837E9CA8 — a device control was destroyed: toggle it if its definition's
 * control.trigger (definition +0x292) equals 1 (destruction-triggered). */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/control_definition.h"
#include "headers/control_trigger.h"
#include "headers/blam_data_globals.h"

extern void control_toggle(int control_index);

void control_destroyed(int control_index)
{
    object_datum *control = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, control_index)->datum;
    control_definition *definition = TAG_GET(control_definition, control->definition_index);
    if (definition->control.trigger == _control_trigger_destruction)
        control_toggle(control_index);
}
