/* hs_tokens_enumerate @0x8372F7E0 — collect, into `results`, the names of all HaloScript tokens of the
 * requested kinds (functions, globals, script names, object names, etc.) that match `substring`, then sort
 * them alphabetically. `type_flags` is a bitmask selecting which of the 18 token enumerators to run; each
 * enumerator appends matching names (respecting `console_flags_to_test`) up to `maximum_count`. Returns the
 * number of tokens collected.
 *
 * See headers/hs_token_type.h for the 18 kinds (NUMBER_OF_HS_TOKEN_TYPES).
 *
 * The enumeration state (search substring, result pointer, running count, maximum) is held in globals
 * (the engine's enumerator callbacks read them). The two pointer globals are properly pointer-typed
 * (canonical defs in src/data/, decls in blam_data_globals.h) — they were previously `int` holding
 * pointers (x64 truncation hazard, batch-13 finding, now resolved). */

#include <stdint.h>
#include "headers/hs_token_type.h"
#include "headers/blam_data_globals.h"

extern __int16 hs_tokens_enumerate_result_count;          /* running result count */
extern char empty_string[];           /* empty-string fallback */
extern __int16 hs_console_flags_to_test;          /* console-flags-to-test */
extern void (*hs_token_enumerators[NUMBER_OF_HS_TOKEN_TYPES])(void);
extern int alphabetize(const char **string1, const char **string2);
extern void qsort(void *base, unsigned int count, unsigned int size, int (__fastcall *compare)(const void *, const void *));

int16_t hs_tokens_enumerate(const char *substring, int type_flags, const char **results, int16_t maximum_count, uint16_t console_flags_to_test)
{
    enumeration_maximum_count = maximum_count;
    hs_tokens_enumerate_result_count = 0;
    hs_tokens_enumerate_results = results;
    hs_tokens_enumerate_substring = substring ? substring : empty_string;
    hs_console_flags_to_test = console_flags_to_test;

    for ( int kind = 0; kind < NUMBER_OF_HS_TOKEN_TYPES; kind = (__int16)(kind + 1) )
    {
        if ( ((1 << kind) & type_flags) != 0 )
            hs_token_enumerators[kind]();
    }

    qsort(results, hs_tokens_enumerate_result_count, 4u, (int (__fastcall *)(const void *, const void *))alphabetize);
    hs_tokens_enumerate_results = 0;  /* NULL out the enumeration-state pointer */
    return hs_tokens_enumerate_result_count; /* attest: lhz load; callers extsh -> int16_t */
}
