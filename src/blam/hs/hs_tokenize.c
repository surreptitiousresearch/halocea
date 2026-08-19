/* hs_tokenize @0x83778560 — allocate a fresh syntax node for the token at the tokenizer's cursor, classify it
 * as primitive (flag 1) unless it opens with '(' (a group), and dispatch to the matching sub-tokenizer.
 * Returns the new node index, or -1 (with hs_compile_globals.__noop set) if no node could be allocated. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tokenizer.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void hs_tokenize_primitive(hs_tokenizer *state, int expression_index);
extern void hs_tokenize_nonprimitive(hs_tokenizer *state, int expression_index);

int hs_tokenize(hs_tokenizer *state)
{
    int expression_index = datum_new(hs_syntax_data);
    if ( expression_index == -1 )
    {
        hs_compile_globals.__noop = "i couldn't allocate a syntax node.";
        return -1;
    }

    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    node->flags = 0;
    node->type = hs_unparsed;
    node->___u1.function_index = -1;
    node->next_node_index = -1;
    /* flags = 1 for a primitive token, 0 for a group; the decompiler's arithmetic reduces to (cursor != '(') */
    node->flags = (*state->cursor == '(') ? 0 : (1u << _hs_syntax_node_primitive_bit);

    if ( (node->flags & (1u << _hs_syntax_node_primitive_bit)) != 0 )
        hs_tokenize_primitive(state, expression_index);
    else
        hs_tokenize_nonprimitive(state, expression_index);
    return expression_index;
}
