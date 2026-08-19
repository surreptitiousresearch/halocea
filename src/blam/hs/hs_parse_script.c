/* hs_parse_script @0x83776498 — parse a script-name atom. Resolves the token to a script index via
 * hs_find_script_by_name and stores it in the node's data field (word); an unknown name is an error. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_find_script_by_name(const char *name);

int hs_parse_script(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int16_t script_index = hs_find_script_by_name(&hs_compile_globals.compiled_source[node->source_offset]);

    if ( script_index == -1 )
    {
        hs_compile_globals.__noop = "this is not a valid script name.";
        hs_compile_globals.error_offset = node->source_offset;
        return 0;
    }
    /* DEVIATION: endian-portable respelling of the BE high-halfword store `sth r11, 0x10(r30)` @0x837764F8
     * (was an *(int16_t*)&node->data pun); hs_cast extracts (int16_t)(value >> 16) 2026-08-18 */
    node->data = (unsigned int)((uint16_t)script_index) << 16;
    return 1;
}
