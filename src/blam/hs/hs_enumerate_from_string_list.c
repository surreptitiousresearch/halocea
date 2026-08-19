#include <stdint.h>
/* hs_enumerate_from_string_list @0x8372F200 — register a slice [start_index, count) of a static string
 * table with the HaloScript tokenizer's enumeration.
 *
 * DEVIATION (sibling-reuse): the compiled body inlines hs_tokens_enumerate_add_string verbatim (capacity
 * guard, inline strlen of the search substring, strnicmp prefix test, append); the recovered helper is
 * called instead of duplicating that logic. */

extern void hs_tokens_enumerate_add_string(const char *string);

void hs_enumerate_from_string_list(const char **strings, int16_t start_index, int16_t count)
{
    for ( int16_t i = start_index; i < count; i++ )
        hs_tokens_enumerate_add_string(strings[i]);
}
