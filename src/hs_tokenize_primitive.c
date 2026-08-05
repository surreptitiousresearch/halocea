/* hs_tokenize_primitive @0x837777B0 — scan one primitive token at the cursor and record its source offset on
 * the node. A double-quoted constant is read to the closing quote (reporting "unterminated" at end of input)
 * and NUL-terminated past the quote; a bare token is read until whitespace, EOL, ')' or ';'. Outside console
 * mode the token text is lower-cased in place. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_tokenizer.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern const char whitespace_characters[];
extern const char eol_characters[];
extern char *strlwr(char *string);

static int char_in_pair(int ch, const char *pair)
{
    for ( int i = 0; i < 2; i = (int16_t)(i + 1) )
        if ( ch == pair[i] )
            return 1;
    return 0;
}

void hs_tokenize_primitive(hs_tokenizer *state, int expression_index)
{
    char *start = state->cursor;
    hs_syntax_node *node = &HS_SYNTAX_NODE(expression_index);

    if ( *state->cursor == '"' )
    {
        state->cursor = start + 1;
        node->source_offset = state->cursor - hs_compile_globals.compiled_source;
        if ( *state->cursor )
        {
            while ( *state->cursor != '"' )
            {
                ++state->cursor;
                if ( !*state->cursor )
                    break;
            }
        }
        if ( !*state->cursor )
        {
            hs_compile_globals.__noop = "this quoted constant is unterminated.";
            hs_compile_globals.error_offset = node->source_offset - 1;
        }
        *state->cursor++ = 0;
    }
    else
    {
        node->source_offset = start - hs_compile_globals.compiled_source;
        if ( *state->cursor )
        {
            while ( 1 )
            {
                char *here = state->cursor;
                int ch = (unsigned char)*state->cursor;
                if ( ch == ')' || ch == ';' )
                    break;
                if ( char_in_pair(ch, whitespace_characters) || char_in_pair(ch, eol_characters) )
                    break;
                state->cursor = here + 1;
                if ( !here[1] )
                    break;
            }
        }
    }

    if ( !hs_console_mode )
        strlwr(&hs_compile_globals.compiled_source[node->source_offset]);
}
