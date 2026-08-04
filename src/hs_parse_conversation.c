/* hs_parse_conversation @0x83776B60 — parse an AI-conversation name (scenario.ai_conversations, 116-byte
 * elements; name at offset 0). */

#include <stdint.h>
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_conversation(int expression_index)
{
    return hs_parse_scenario_datum(expression_index, 0, &global_scenario->ai_conversations, 116);
}
