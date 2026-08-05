/* console_complete @ 0x83722530 — tab-completion for the console input. Finds the token
 * under the caret (after the last space / '(' / '"'), enumerates matching hs script symbols,
 * lists them (compact grid when there are many), then completes the input up to the longest
 * common prefix of all matches.
 *
 * "profile_load" is deliberately excluded from completion. Common-prefix math is reproduced
 * faithfully from the decompiler. */

#include <stdint.h>
#include "headers/console_globals.h"

extern char  *strrchr(const char *s, int c);
extern char  *strncpy(char *dst, const char *src, unsigned int n);
extern int    tolower(int c);
extern int16_t hs_tokens_enumerate(const char *substring, int type_flags, const char **results, int16_t maximum_count, uint16_t console_flags_to_test);
extern void console_printf_only_if_open(uint8_t clear, const char *format, ...);

extern const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

void console_complete(void)
{
    char *token_start;
    char *after_space;
    char *after_paren;
    char *after_quote;
    int16_t match_count;
    int16_t common_prefix_len;
    char compact_grid;
    const char *tokens[288];
    char line[1024];
    int printed;
    int i;

    token_start = console_globals.input_state.result;
    after_space = strrchr(console_globals.input_state.result, ' ') + 1;
    after_paren = strrchr(console_globals.input_state.result, '(') + 1;
    after_quote = strrchr(console_globals.input_state.result, '"');
    if ( console_globals.input_state.result <= after_space )
        token_start = after_space;
    if ( token_start <= after_paren )
        token_start = after_paren;
    if ( token_start <= after_quote + 1 )
        token_start = after_quote + 1;

    match_count = hs_tokens_enumerate(token_start, '(', tokens, 256, 1u);
    if ( !match_count )
        return;

    common_prefix_len = 0x7FFF;
    compact_grid = (match_count > 16);   /* ((count>=0)+(count<=16))&1 == (count>16) */

    /* single exact "profile_load" match: do nothing */
    if ( match_count == 1 )
    {
        const char *a = tokens[0];
        const char *b = "profile_load";
        int diff;
        do
        {
            diff = (unsigned char)*a - (unsigned char)*b;
            if ( !*a )
                break;
            ++a; ++b;
        }
        while ( !diff );
        if ( !diff )
            return;
    }

    line[0] = 0;
    console_printf_only_if_open(0, empty_string);
    printed = 0;
    for ( i = 0; i < match_count; i = (int16_t)(i + 1) )
    {
        const char *token = tokens[i];
        const char *a = token;
        const char *b = "profile_load";
        int diff;
        const char *p;
        int token_len_minus_1;
        int16_t compare_limit;
        int prefix_idx;
        int last_match;

        do
        {
            diff = (unsigned char)*a - (unsigned char)*b;
            if ( !*a )
                break;
            ++a; ++b;
        }
        while ( !diff );
        if ( !diff )
            continue;   /* skip profile_load */

        /* token length - 1 */
        p = token;
        while ( *p++ )
            ;
        token_len_minus_1 = (int)(p - token - 2);

        compare_limit = common_prefix_len;
        if ( common_prefix_len > token_len_minus_1 )
            compare_limit = (int16_t)token_len_minus_1;

        /* longest common prefix between tokens[0] and this token, up to compare_limit */
        prefix_idx = 0;
        if ( tolower(*token) == tolower(*tokens[0]) )
        {
            int idx = 0;
            do
            {
                if ( idx > compare_limit )
                    break;
                prefix_idx = (int16_t)(idx + 1);
                idx = prefix_idx;
                last_match = tolower(tokens[0][prefix_idx]);
            }
            while ( tolower(token[prefix_idx]) == last_match );
        }
        common_prefix_len = (int16_t)(prefix_idx - 1);

        if ( compact_grid )
        {
            /* append "<token>|t" to the current grid line; flush every 4 columns */
            char *end = line;
            while ( *end++ )
                ;
            --end;
            { const char *s = token; do { *end++ = *s; } while ( *s++ ); }
            end = line;
            while ( *end++ )
                ;
            --end;
            { const char *s = "|t"; do { *end++ = *s; } while ( *s++ ); }
            if ( printed % 4 == 3 )
            {
                console_printf_only_if_open(0, line);
                line[0] = 0;
            }
        }
        else
        {
            console_printf_only_if_open(0, tokens[i]);
        }
        ++printed;
    }

    if ( compact_grid && (printed - 1) % 4 != 3 )
        console_printf_only_if_open(0, line);

    if ( common_prefix_len != 0x7FFF )
    {
        strncpy(token_start, tokens[0], common_prefix_len + 1);
        token_start[common_prefix_len + 1] = 0;
        console_globals.input_state.edit.insertion_point_index =
            (int16_t)(token_start - console_globals.input_state.result + common_prefix_len + 1);
    }
}
