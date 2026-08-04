/* hs_tokenize_nonprimitive @0x83778638 — tokenize the body of a "(...)" group: record the opening paren's
 * source offset, then repeatedly skip whitespace (NUL-terminating each token in place) and tokenize child
 * expressions, linking them through the group node's child list, until a closing ')' is reached. Reports
 * "unmatched left parenthesis" at end of input and "empty expression" if the group held no children. */

#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tokenizer.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void skip_whitespace(char **c);
extern int hs_tokenize(hs_tokenizer *state);

void hs_tokenize_nonprimitive(hs_tokenizer *state, int expression_index)
{
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);
    int *child_link = &node->data;      /* first child goes in node->data, then each child's next_node_index */

    node->source_offset = state->cursor - hs_compile_globals.compiled_source;
    ++state->cursor;                    /* consume '(' */

    const char *error = hs_compile_globals.__noop;
    if ( !hs_compile_globals.__noop )
    {
        while ( 1 )
        {
            char *token_start = state->cursor;
            skip_whitespace(&state->cursor);
            if ( state->cursor != token_start )
                *token_start = 0;       /* terminate the preceding token */

            if ( !*state->cursor )
            {
                error = "this left parenthesis is unmatched.";
                hs_compile_globals.__noop = "this left parenthesis is unmatched.";
                hs_compile_globals.error_offset = node->source_offset;
                goto done;
            }
            if ( *state->cursor == ')' )
                break;

            int child = hs_tokenize(state);
            *child_link = child;
            if ( child != -1 )
                child_link = &HS_SYNTAX_NODE(child).next_node_index;
            error = hs_compile_globals.__noop;
            if ( hs_compile_globals.__noop )
                goto done;
        }
        *state->cursor++ = 0;           /* terminate at the ')' */
        error = hs_compile_globals.__noop;
    }

done:
    if ( child_link == &node->data && !error )
    {
        hs_compile_globals.__noop = "this expression is empty.";
        hs_compile_globals.error_offset = node->source_offset;
    }
}
