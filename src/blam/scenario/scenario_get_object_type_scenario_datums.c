/* scenario_get_object_type_scenario_datums @0x83702B50 — return the scenario's placement tag_block for a given
 * object type, optionally reporting the per-element size. Both the block offset within the scenario tag and the
 * element size come from the per-type definition. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/tag_block.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


tag_block *scenario_get_object_type_scenario_datums(scenario *scenario, int16_t object_type, int *size)
{
    object_type_definition *definition = object_type_definitions[object_type];
    if (size)
        *size = definition->placement_tag_block_element_size;
    return (tag_block *)((char *)scenario + definition->placement_tag_block_offset);
}
