/* field_properties_dispose @0x837A0B30 — DEVIATION: same bctr-tail-call class as
 * field_type_pointer_size_calculator.c, but here disasm_range(0x837A0B30,0x837A0B90) shows r3 is simply
 * never touched between function entry and `bctrl` — it still holds the original
 * field_properties_definition argument, matching the destructor's established
 * `void (*)(const _field_properties_definition*)` prototype (field_type_definition.h). */

#include "headers/field_properties_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


void field_properties_dispose(_field_properties_definition *const field_properties_definition)
{
    if ( field_properties_definition->initialized == 1 )
    {
        message_delta_global_field_type_list[field_properties_definition->type].destructor(field_properties_definition);
        field_properties_definition->initialized = 0;
    }
}
