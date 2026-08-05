/* scenario_get_sky @0x83703700 — resolve a scenario sky reference index to its loaded sky tag definition, or
 * null if the index is out of range or the reference is empty. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_globals.h"
#include "headers/tag_reference.h"
#include "headers/sky.h"
#include "headers/global_tag_instances.h"

sky *scenario_get_sky(int16_t sky_index)
{
    int sky_definition_index = -1;
    if (sky_index >= 0 && sky_index < global_scenario->sky_references.count)
        sky_definition_index = ((tag_reference *)global_scenario->sky_references.address)[sky_index].index;

    if (sky_definition_index == -1)
        return nullptr;
    return TAG_GET(sky, sky_definition_index);
}
