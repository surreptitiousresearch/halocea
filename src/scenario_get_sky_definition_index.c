#include <stdint.h>
#include "headers/scenario.h"
#include "headers/tag_reference.h"
#include "headers/blam_data_globals.h"


int scenario_get_sky_definition_index(int16_t sky_index)
{
    if ( sky_index >= 0 && sky_index < global_scenario->sky_references.count )
        return ((tag_reference *)global_scenario->sky_references.address)[sky_index].index;
    return -1;
}
