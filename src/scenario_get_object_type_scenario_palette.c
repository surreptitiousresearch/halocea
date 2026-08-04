/* scenario_get_object_type_scenario_palette @0x83702B88 — return the scenario's palette tag_block for a given
 * object type. The per-type definition records the byte offset of that type's palette block within the scenario
 * tag. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/tag_block.h"
#include "headers/object_type_definition.h"
#include "headers/blam_data_globals.h"


tag_block * scenario_get_object_type_scenario_palette(scenario *scenario, int16_t object_type)
{
    return (tag_block *)((char *)scenario + object_type_definitions[object_type]->palette_tag_block_offset);
}
