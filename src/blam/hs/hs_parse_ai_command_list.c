/* hs_parse_ai_command_list @0x83776B30 — parse an AI command-list name (scenario.ai_command_lists, 96-byte
 * elements; name at offset 0). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_ai_command_list(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 0, &global_scenario->ai_command_lists, 96);
}
