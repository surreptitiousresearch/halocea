/* control_place @0x837E9A78 — apply a scenario control placement's device/switch flags to its runtime object:
 * registers the device sub-block, then translates the placement's "usable from both sides"/"one-sided" flags
 * (bits 0x1/0x10) into the object's runtime flags (+532 bits 0x1/0x2), and stores the HUD override string
 * index (1-based in the tag, stored 0-based at +536).
 *
 * Object header lookup matches the established salted object-header idiom (see vehicle_reset.c). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/control_datum.h"
#include "headers/control_datum_flags.h"
#include "headers/scenario_control_datum.h"
#include "headers/scenario_control_datum_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/scenario_device_datum.h"
extern void device_add_scenario_information(unsigned int device_index, scenario_device_datum *scenario_device);

void control_place(int control_index, scenario_control_datum *scenario_control)
{
    control_datum *control = (control_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, control_index)->datum;

    device_add_scenario_information(control_index, &scenario_control->device);

    if ( (scenario_control->flags & (1u << _scenario_control_usable_from_both_sides_bit)) != 0 )
        control->control.flags |= (1u << _control_usable_from_both_sides_bit);
    /* no DB enum found: only bit0 is named in scenario_control/control flag enums
     * ($ED7B6B08.., $CDFC7796..); bit4 (source) and bit1 (dest) have no named value. */
    if ( (scenario_control->flags & 0x10) != 0 )
        control->control.flags |= 2u;

    control->control.hud_override_index = scenario_control->hud_override_string_list_index - 1;
}
