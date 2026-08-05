/* hs_parse_ai @0x83776AA0 — parse an AI atom (an encounter or squad). Resolves the token via
 * ai_index_from_string, which writes the packed AI index into the node's data field; an unresolved name is an
 * error. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

#include "headers/scenario.h"
extern uint8_t ai_index_from_string(scenario *scenario, const char *ai_string, unsigned int *ai_index_reference);

int hs_parse_ai(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int result = ai_index_from_string(global_scenario,
                                      &hs_compile_globals.compiled_source[node->source_offset],
                                      &node->data);
    if ( !(unsigned char)result )
    {
        hs_compile_globals.__noop = "this is not a valid ai encounter or squad.";
        hs_compile_globals.error_offset = node->source_offset;
    }
    return result;
}
