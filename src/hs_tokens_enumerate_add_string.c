#include "headers/blam_data_globals.h"
/* hs_tokens_enumerate_add_string @0x8372F160 — hs_tokens_enumerate callback helper: appends `string` to the
 * in-progress results array if there's still room and it's not a duplicate of the search substring's leading
 * prefix (case-insensitive compare over the substring's length). Shares the file-scope enumeration state
 * globals established in hs_tokens_enumerate.c. */

extern __int16 hs_tokens_enumerate_result_count;          /* running result count */

extern int strnicmp(const char *a, const char *b, unsigned int n);

void hs_tokens_enumerate_add_string(const char *string)
{
    if ( hs_tokens_enumerate_result_count >= enumeration_maximum_count )
        return;

    const char *substring = hs_tokens_enumerate_substring;
    unsigned int substring_length = 0;
    while ( substring[substring_length] )
        substring_length++;

    if ( !strnicmp(string, substring, substring_length) )
        hs_tokens_enumerate_results[hs_tokens_enumerate_result_count++] = string;
}
