/* hs_parse_string @0x83776458 — parse a string atom. The value is simply a pointer to the token text within
 * the compiled source buffer; stored into the node's data field. (hs_compile_finish later interns it.) */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


int hs_parse_string(int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    node->data = (unsigned int)&hs_compile_globals.compiled_source[node->source_offset];
    return 1;
}
