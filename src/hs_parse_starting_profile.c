/* hs_parse_starting_profile @0x83776B48 — parse a starting-profile name (scenario.starting_profiles, 104-byte
 * elements; name at offset 0). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_starting_profile(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 0, &global_scenario->starting_profiles, 104);
}
