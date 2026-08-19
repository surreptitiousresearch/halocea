/* global_structure_bsp_tag_index_get @0x83703450 — resolved tag index of the currently-active structure BSP,
 * looked up from the scenario's structure_bsp_references block by global_structure_bsp_index. */

#include "headers/scenario.h"
#include "headers/scenario_structure_bsp_reference.h"
#include "headers/blam_data_globals.h"


int global_structure_bsp_tag_index_get(void)
{
    scenario_structure_bsp_reference *references = (scenario_structure_bsp_reference *)global_scenario->structure_bsp_references.address;
    return references[global_structure_bsp_index].structure_bsp.index;
}
