/* hs_parse_sleep @0x83779710 — parse (sleep <ticks> [<script>]). The first argument is a short tick count
 * (type 7); the optional second argument is a script name (type 10). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_sleep(int16_t function_index, int expression_index)
{
    hs_syntax_node *group = &HS_SYNTAX_NODE(expression_index);
    int ticks = HS_SYNTAX_NODE(group->data).next_node_index;

    if ( ticks == -1 )
    {
        hs_compile_globals.__noop = "the sleep call requires a time and, optionally, a script name.";
        hs_compile_globals.error_offset = group->source_offset;
    }
    else if ( hs_parse(ticks, hs_type_short_integer) )
    {
        int script = HS_SYNTAX_NODE(ticks).next_node_index;
        if ( script == -1 || hs_parse(script, hs_type_script) )
            return 1;
    }
    return 0;
}
