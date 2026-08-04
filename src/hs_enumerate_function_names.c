/* hs_enumerate_function_names @0x8372F3FC — one of hs_tokens_enumerate's per-kind callbacks: offers every
 * built-in HaloScript function's name to the in-progress results collection.
 *
 * DEVIATION: the decompiler inlined hs_tokens_enumerate_add_string's own substring-length/strnicmp/append
 * logic verbatim; restored as a call to that already-recovered helper, which is the same logic. */

#include "headers/hs_function_definition.h"

#define HS_FUNCTION_COUNT 471

extern void hs_tokens_enumerate_add_string(const char *string);

void hs_enumerate_function_names(void)
{
    for ( int i = 0; i < HS_FUNCTION_COUNT; ++i )
        hs_tokens_enumerate_add_string(hs_function_table[i]->name);
}
