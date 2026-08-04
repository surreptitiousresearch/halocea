/* hs_parse_device_group @0x83776A88 — parse a device-group name (scenario.device_groups, 52-byte elements;
 * name at offset 0). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_device_group(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 0, &global_scenario->device_groups, 52);
}
