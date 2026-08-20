/* objects_place @0x836EE188 — place every scenario object for a freshly loaded map. The initial_placement
 * flag is raised across the call. Its only reader corpus-wide is object_new_from_scenario, where it gates
 * out scenario objects carrying _scenario_object_placement_not_automatic_bit; nothing else in the object
 * system consults it, and the creation effect still fires (bl effect_new_from_object @0x836F1CD4 is
 * unconditional in object_new_with_datum_role_control). */

#include "headers/object_globals.h"
#include "headers/scenario.h"

extern void object_types_place_all(scenario *scenario_ptr);

void objects_place(void)
{
    object_globals->initial_placement = 1;
    object_types_place_all(global_scenario);
    object_globals->initial_placement = 0;
}
