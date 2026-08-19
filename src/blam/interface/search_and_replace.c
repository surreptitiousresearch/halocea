/* search_and_replace @0x83732550 — replace every occurrence of `search` with `replace` in the
 * null-terminated wide string pointed to by `*in_buffer`, in place. If `replace` is no longer than
 * `search`, each match is replaced and the tail memmove'd left without reallocating. If `replace` is
 * longer, the buffer is grown once (via `pool_resize_pointer`) to fit every match, then each match is
 * replaced with the tail memmove'd right. Returns the number of replacements made, or -1 on allocation
 * failure in the grow path. */

#include <stdint.h>
#include <string.h>
#include "headers/stack_memory_pool.h"
#include "headers/blam_data_globals.h"


extern unsigned int ustrlen(const wchar_t *string);
extern wchar_t *ustrstr(const wchar_t *haystack, const wchar_t *needle);
extern void *pool_resize_pointer(stack_memory_pool *pool, void *p, unsigned int new_size);
/* memcpy declared by <string.h> */

int search_and_replace(const wchar_t *search, const wchar_t *replace, uint16_t **in_buffer)
{
    int replacements = 0;

    if (!in_buffer || !*in_buffer)
        return replacements;

    uint16_t *buffer = *in_buffer;
    int search_len = ustrlen(search);
    int replace_len = ustrlen(replace);
    unsigned int remaining_len = ustrlen(buffer) + 1;

    if (replace_len > search_len)
    {
        int grow = replace_len - search_len;
        uint16_t *match = ustrstr(buffer, search);
        if (match)
        {
            for (uint16_t *scan = match; scan; scan = ustrstr(&scan[search_len], search))
                ++replacements;

            if (replacements > 0)
            {
                uint16_t *new_buffer =
                    pool_resize_pointer(widget_memory_pool, buffer, 2 * (grow * replacements + remaining_len));
                if (!new_buffer)
                    return -1;

                for (uint16_t *i = ustrstr(new_buffer, search); i; i = ustrstr(new_buffer, search))
                {
                    memmove(&i[replace_len], &i[search_len], 2 * (remaining_len - (unsigned int)(i - new_buffer) - search_len));
                    memcpy(i, replace, 2 * replace_len);
                    remaining_len += grow;
                }

                *in_buffer = new_buffer;
            }
        }
        return replacements;
    }

    int shrink = search_len - replace_len;
    uint16_t *match = ustrstr(buffer, search);
    if (!match)
        return replacements;

    do
    {
        ++replacements;
        memcpy(match, replace, 2 * replace_len);
        if (shrink > 0)
        {
            memmove(&match[replace_len], &match[search_len], 2 * (remaining_len - (unsigned int)(match - buffer) - replace_len));
            remaining_len -= shrink;
        }
        match = ustrstr(buffer, search);
    } while (match);

    return replacements;
}
