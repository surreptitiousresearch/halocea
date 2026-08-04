/* skip_whitespace @0x83777978 — advance *c past whitespace, end-of-line characters, line comments (";" to
 * EOL) and block comments (";*" ... "*;"). Stops at the first significant character (leaving *c on it) or at
 * end of string. An unterminated block comment sets hs_compile_globals.__noop. */

#include "headers/hs_compile_globals.h"

extern const char whitespace_characters[];   /* 2 entries */
extern const char eol_characters[];           /* 2 entries */

/* comment-scan states */
enum { SCAN_NORMAL = 0, SCAN_LINE_COMMENT = 1, SCAN_BLOCK_COMMENT = 2, SCAN_DONE = 3 };

static int char_in_pair(int ch, const char *pair)
{
    for ( int i = 0; i < 2; i = (__int16)(i + 1) )
        if ( ch == pair[i] )
            return 1;
    return 0;
}

void skip_whitespace(char **c)
{
    int state = SCAN_NORMAL;
    do
    {
        if ( state == SCAN_NORMAL )
        {
            char *cursor = *c;
            int ch = (unsigned char)**c;
            char *next;
            if ( ch == ';' )
            {
                state = SCAN_LINE_COMMENT;
                *c = cursor + 1;
                if ( cursor[1] != '*' )
                    continue;
                next = cursor + 2;
                state = SCAN_BLOCK_COMMENT;
            }
            else
            {
                if ( !char_in_pair(ch, whitespace_characters) && !char_in_pair(ch, eol_characters) )
                {
                    state = SCAN_DONE;
                    continue;
                }
                next = cursor + 1;
            }
            *c = next;
            continue;
        }

        if ( state != SCAN_LINE_COMMENT )       /* block comment */
        {
            if ( !**c )
            {
                state = SCAN_DONE;
                hs_compile_globals.__noop = "unterminated comment.";
                continue;
            }
            char *next = *c + 1;
            if ( **c == '*' && (*c)[1] == ';' )
            {
                state = SCAN_NORMAL;
                ++*c;
                next = *c + 1;
            }
            *c = next;
            continue;
        }

        /* line comment: consume to end of line */
        char ch = **c;
        if ( ch )
        {
            int is_eol = char_in_pair(ch, eol_characters);
            ++*c;
            if ( is_eol )
                state = SCAN_NORMAL;
        }
        else
        {
            state = SCAN_DONE;
        }
    }
    while ( state != SCAN_DONE );
}
