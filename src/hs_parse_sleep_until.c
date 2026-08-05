/* hs_parse_sleep_until @0x837797F8 — parse (sleep_until <condition> [<period> [<timeout>]]). The condition is
 * boolean (type 5); the optional period is a short (type 7) and the optional timeout a long (type 8). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_sleep_until(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);
    int condition = HS_SYNTAX_NODE(group->data).next_node_index;

    if ( condition == -1 )
    {
        hs_compile_globals.__noop = "the sleep_until call requires a condition and, optionally, a period.";
        hs_compile_globals.error_offset = group->source_offset;
        return 0;
    }

    int period = HS_SYNTAX_NODE(condition).next_node_index;
    int result = hs_parse(condition, hs_type_boolean);
    if ( (unsigned char)result && period != -1 )
    {
        int timeout = HS_SYNTAX_NODE(period).next_node_index;
        result = hs_parse(period, hs_type_short_integer);
        if ( (unsigned char)result && timeout != -1 )
            return hs_parse(timeout, hs_type_long_integer);
    }
    return result;
}
