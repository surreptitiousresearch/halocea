/* string_list_find @0x83776E80 — return the index of `string` within `string_list` (a list of `list_count`
 * C strings), or -1 if absent. All 3 callers extsh the result — declared return is int16_t.
 *
 * Deviation: the decompiler inlines the per-entry comparison as a byte loop; reproduced with strcmp. */

#include <stdint.h>

extern int strcmp(const char *a, const char *b);

int16_t string_list_find(const char *string, int16_t list_count, const char **string_list)
{
    for ( int16_t i = 0; i < list_count; i = (int16_t)(i + 1) )
        if ( !strcmp(string, string_list[i]) )
            return i;
    return -1;
}
