/* hs_parse_trigger_volume @0x83776A10 — parse a trigger-volume name (scenario.trigger_volumes, 96-byte
 * elements; name at offset 4). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_trigger_volume(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 4, &global_scenario->trigger_volumes, 96);
}
