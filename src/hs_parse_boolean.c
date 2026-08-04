/* hs_parse_boolean @0x83776080 — parse a boolean atom. The token text (compiled_source[source_offset]) is
 * matched case-sensitively against the accepted spellings: "false"/"off"/"0" => 0, "true"/"on"/"1" => 1.
 * Anything else is an error. The parsed value is written to the node's data field (low byte).
 *
 * Deviation: the decompiler inlines each spelling test as a byte-compare loop; reproduced with strcmp. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int strcmp(const char *a, const char *b);

int hs_parse_boolean(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    const char *token = &hs_compile_globals.compiled_source[node->source_offset];

    if ( !strcmp(token, "false") || !strcmp(token, "off") || !strcmp(token, "0") )
    {
        *(unsigned char *)&node->data = 0;
        return 1;
    }
    if ( !strcmp(token, "true") || !strcmp(token, "on") || !strcmp(token, "1") )
    {
        *(unsigned char *)&node->data = 1;
        return 1;
    }
    hs_compile_globals.__noop = "i expected \"true\" or \"false\".";
    hs_compile_globals.error_offset = node->source_offset;
    return 0;
}
