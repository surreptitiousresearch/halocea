/* objects_place @0x836EE188 — place every scenario object for a freshly loaded map. The initial_placement flag
 * is raised across the call so object creation uses map-load semantics (e.g. no creation effects). */

#include "headers/object_globals.h"
#include "headers/scenario.h"

extern void object_types_place_all(scenario *scenario_ptr);

void objects_place(void)
{
    object_globals->initial_placement = 1;
    object_types_place_all(global_scenario);
    object_globals->initial_placement = 0;
}
