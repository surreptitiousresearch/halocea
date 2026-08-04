/* hs_parse_cutscene_flag @0x83776A28 — parse a cutscene-flag name (scenario.cutscene_flags, 92-byte elements;
 * name at offset 4). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_cutscene_flag(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 4, &global_scenario->cutscene_flags, 92);
}
