/* hs_parse_cutscene_recording @0x83776A70 — parse a recorded-animation name (scenario.recorded_animations,
 * 64-byte elements; name at offset 0). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_cutscene_recording(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 0, &global_scenario->recorded_animations, 64);
}
